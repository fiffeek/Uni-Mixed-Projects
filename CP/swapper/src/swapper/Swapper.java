package swapper;

import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Swapper<E> {

    private HashSet<E> elements;
    private ConcurrentHashMap<E, List<Waiter>> elementsAvailability;
    private Lock lock = new ReentrantLock();

    private int waitersNum = 0;
    private final boolean DEBUG = false;

    public Swapper() {
        this.elements = new HashSet<>();
        this.elementsAvailability = new ConcurrentHashMap<>();
    }

    /**
     * Returns how many elements of removed are
     * currently in a swapper
     * @param removed - set of unique elements to remove
     * @return number of elements present both in removed and in a swapper
     */
    private int howManyElemInSet(HashSet<E> removed) {
        int res = 0;

        for (E elem: removed) {
            if (elements.contains(elem)) {
                res++;
            }
        }

        return res;
    }

    /**
     * Add remove conditions for a thread that called swap function
     * @param removed - set of elements to remove
     * @param waiter - a class on which the thread waits
     */
    private void addRemoveConditions(HashSet<E> removed, Waiter waiter) {
        for (E elem: removed) {
            if (elementsAvailability.containsKey(elem)) {
                elementsAvailability.get(elem).add(waiter);
            } else {
                LinkedList<Waiter> waiters = new LinkedList<>();
                waiters.add(waiter);

                elementsAvailability.put(elem, waiters);
            }
        }
    }

    /**
     * Add [k] (either 1 or -1) to every waiter
     * which is present on a list of one of the elements
     * from setOfElems
     * @param setOfElems - set of elements to add waiting
     * @param k - 1 or -1 to add
     */
    private void changeConditions(HashSet<E> setOfElems, int k) {
        for (E elem: setOfElems) {
            if (!elementsAvailability.containsKey(elem)) continue;

            for (Waiter waiter: elementsAvailability.get(elem)) {
                waiter.addToElems(k);
            }
        }
    }

    /**
     * Either after throwing an error or finishing
     * the swapper is going to remove the 'remove' conditions
     * of the actual thread, meaning that it has finished
     * @param waiter - Waiter class to remove
     */
    private void removeConditions(Waiter waiter) {
        for (List<Waiter> waiters: elementsAvailability.values()) {
            waiters.remove(waiter);
        }
    }

    /**
     * Simple log function, which logs some info to stderr
     * when the parameter DEBUG is on
     * @param deb - the string to push on stderr
     */
    private void logDebug(String deb) {
        if (!DEBUG) return;
        System.err.println(deb);
    }

    /**
     * Cleans up after the process was interrupted
     * @param removedSet - set which was removed from the swapper
     * @param addedSet - set which was added to the swapper
     */
    private void cleanUp(HashSet<E> removedSet, HashSet<E> addedSet, HashSet<E> addedSetWithRepetitions) {
        changeConditions(addedSet, 1);
        elements.removeAll(addedSetWithRepetitions);
        changeConditions(removedSet, -1);
        elements.addAll(removedSet);
    }

    /**
     * Waits until all elements of a removed collection are present
     * on a swapper, then (atomically) removes that elements and adds
     * the elements from the 'added' collection
     * @param removed - collection to remove from the swapper
     * @param added - collection to add to the swapper
     */
    public void swap(Collection<E> removed, Collection<E> added) throws InterruptedException  {
        HashSet<E> removedSet = new HashSet<>(removed);
        HashSet<E> addedSet = new HashSet<>(added);
        HashSet<E> addedSetWithRepetitions = new HashSet<>(added);

        lock.lockInterruptibly();

        Waiter waiter = new Waiter(
                removedSet.size() - howManyElemInSet(removedSet),
                waitersNum++,
                lock.newCondition()
        );
        addRemoveConditions(removedSet, waiter);
        logDebug("Added conditions for " + Thread.currentThread().toString());

        while (waiter.elemNumber > 0) {
            try {
                waiter.mutex.await();
            } catch (InterruptedException e) {
                removeConditions(waiter);
                lock.unlock();

                throw e;
            }
        }

        HashSet<E> swapperMinusRemoved = new HashSet<>(elements);
        swapperMinusRemoved.removeAll(removedSet);
        addedSet.removeAll(swapperMinusRemoved);
        boolean exceptionThrow = false;
        logDebug("Actually swapping in " + Thread.currentThread().toString());

        removeConditions(waiter);
        elements.removeAll(removedSet);
        changeConditions(removedSet, 1);
        elements.addAll(addedSetWithRepetitions);
        changeConditions(addedSet, -1);

        if (Thread.currentThread().isInterrupted()) {
            cleanUp(removedSet, addedSet, addedSetWithRepetitions);
            exceptionThrow = true;
        }

       ArrayList<Waiter> waitersToInvoke = new ArrayList<>();

        for (List<Waiter> waiters: elementsAvailability.values()) {
            for (Waiter waiterTemp: waiters) {
                if (waiterTemp.elemNumber == 0) {
                    waitersToInvoke.add(waiterTemp);
                }
            }
        }

        for (Waiter temp: waitersToInvoke) {
            temp.invoke();
        }

        lock.unlock();

        if (exceptionThrow) {
            throw new InterruptedException();
        }
    }

    private static class Waiter {

        int elemNumber;
        Condition mutex;
        private int num;

        Waiter(int elems, int index, Condition mutex) {
            this.elemNumber = elems;
            this.mutex = mutex;
            this.num = index;
        }

        void addToElems(int k) {
            elemNumber += k;
        }

        void invoke() {
            mutex.signal();
        }

        @Override
        public int hashCode() {
            return Integer.hashCode(num);
        }

        @Override
        public boolean equals(Object o) {
            if (!(o instanceof Waiter)) {
                return false;
            }

            return ((Waiter) o).num == this.num;
        }
    }
}