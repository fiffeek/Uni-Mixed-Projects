package swapper;

import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.Semaphore;

public class Swapper<E> {

    public HashSet<E> elements;
    private Semaphore mutex;
    private ConcurrentHashMap<E, List<Waiter>> elementsAvailability;

    private int waitersNum = 0;
    private final boolean DEBUG = false;

    public Swapper() {
        this.elements = new HashSet<>();
        this.mutex = new Semaphore(1);
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
            waiters.removeIf(elem -> elem.equals(waiter));
        }
    }

    /**
     * Simple log funcion, which logs some info to stderr
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
     * Waitis until all elements of a removed collection are present
     * on a swapper, then (atomically) removes that elements and adds
     * the elements from the 'added' collection
     * @param removed - collection to remove from the swapper
     * @param added - collection to add to the swapper
     */
    public void swap(Collection<E> removed, Collection<E> added) throws InterruptedException  {
        // DISCLAIMER: solution with mutex forwarding
        // sections commented with ~ are about that

        // firsly we ensure that there are no duplicates in either of
        // the collections given
        HashSet<E> removedSet = new HashSet<>(removed);
        HashSet<E> addedSet = new HashSet<>(added);
        HashSet<E> addedSetWithRepetitions = new HashSet<>(added);

        // ~ here the swapper acquires a mutex
        // if acquire throws interrupted exception that is fine
        // we have not modified anything yet
        mutex.acquire();

        // we remove from the addSet the elements already
        // added present in swapper (without the ones we will remove)
        HashSet<E> swapperMinusRemoved = new HashSet<>(elements);
        swapperMinusRemoved.removeAll(removedSet);
        addedSet.removeAll(swapperMinusRemoved);

        Waiter waiter = new Waiter(removedSet.size() - howManyElemInSet(removedSet), waitersNum++);
        addRemoveConditions(removedSet, waiter);
        logDebug("Added conditions for " + Thread.currentThread().toString());

        try {
            // ~ if elemNumber is not a zero
            // that means this thread is going to wait on a Waiter
            // below, so we release the mutex to someone else
            if (waiter.elemNumber != 0) {
                mutex.release();
            }

            waiter.mutex.acquire();
            // ~ if the swapper entered this section of code,
            // the thread has the mutex

            logDebug("Invoked " + Thread.currentThread().toString());
        } catch (InterruptedException e) {
            // in this case we need to remove conditions
            removeConditions(waiter);
            mutex.release();
            throw e;
        }

        logDebug("Actually adding in " + Thread.currentThread().toString());

        // if the swapper is in this scope
        // it indicates that it acquired the mutex and the waiter
        // so the elements that the thread has been waiting for are all
        // present on a swapper, so we remove the (empty) condition
        // as well as all the elements from the removedSet
        removeConditions(waiter);
        elements.removeAll(removedSet);

        // now for every element, that the swapper removed
        // we have to update all the Waiters in a lists of that elements
        changeConditions(removedSet, 1);

        // it is time to add the elements and
        // for every element added the swapper changes
        // condition for every Waiter waiting for that element
        elements.addAll(addedSetWithRepetitions);
        changeConditions(addedSet, -1);

        logDebug("Changed conditions");

        // now swapper iterates over every condition
        // checking whether it is possible to grant the access to any
        // other thread (if the number of elements it has been waiting for is 0)
        boolean alreadyInvoked = false;
        for (List<Waiter> waiters: elementsAvailability.values()) {

            for (Waiter waiter_aux: waiters) {

                // if it is and if it has not been invoked so far
                // the swapper invokes that thread and breaks
                // (one thread can affect the other one so the swapper
                // can't actually grant the permission to more than one thread here)
                if (waiter_aux.elemNumber == 0) {
                    logDebug("Invoking waiter");

                    if (Thread.currentThread().isInterrupted()) {
                        cleanUp(removedSet, addedSet, addedSetWithRepetitions);
                        mutex.release();
                        throw new InterruptedException();
                    }

                    // ~ note that the swapper does not release the mutex
                    // instead it it being passed to the next thread
                    waiter_aux.invoke();
                    alreadyInvoked = true;
                    break;
                }
            }

            if (alreadyInvoked) {
                break;
            }
        }

        // ~ if none of the threads were given a mutex
        // the swapper gives it to the world (random one who will want it)
        if (!alreadyInvoked) {

            if (Thread.currentThread().isInterrupted()) {
                cleanUp(removedSet, addedSet, addedSetWithRepetitions);
                mutex.release();
                throw new InterruptedException();
            }
            logDebug("Elements in a set " + elements.size() + " in a " + Thread.currentThread().toString());

            mutex.release();
        }
    }

    private static class Waiter {

        int elemNumber;
        Semaphore mutex;
        private int num;

        Waiter(int elems, int index) {
            this.elemNumber = elems;
            this.mutex = elems == 0 ? new Semaphore(1) : new Semaphore(0);
            this.num = index;
        }

        void addToElems(int k) {
            elemNumber += k;
        }

        void invoke() {
            this.mutex.release();
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