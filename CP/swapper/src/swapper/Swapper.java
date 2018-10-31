package swapper;

import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.Semaphore;

public class Swapper<E> {
    private HashSet<E> elements;
    private Semaphore mutex;
    private ConcurrentHashMap<E, List<Waiter>> elementsAvailability;
    private int waitersNum = 0;
    private final boolean DEBUG = true;

    public Swapper() {
        this.elements = new HashSet<>();
        this.mutex = new Semaphore(1);
        this.elementsAvailability = new ConcurrentHashMap<>();
    }

    private int howManyElemInSet(HashSet<E> removed) {
        int res = 0;

        for (E elem: removed) {
            if (elements.contains(elem)) {
                res++;
            }
        }

        return res;
    }

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

    private void changeConditions(HashSet<E> setOfElems, int k) {
        for (E elem: setOfElems) {
            if (!elementsAvailability.containsKey(elem)) continue;

            for (Waiter waiter: elementsAvailability.get(elem)) {
                waiter.addToElems(k);
            }
        }
    }

    private void removeConditions(Waiter waiter) {
        for (List<Waiter> waiters: elementsAvailability.values()) {
            waiters.removeIf(elem -> elem.equals(waiter));
        }
    }

    private void logDebug(String deb) {
        if (!DEBUG) return;
        System.err.println(deb);
    }

    public void swap(Collection<E> removed, Collection<E> added) throws InterruptedException  {
        HashSet<E> removedSet = new HashSet<>(removed);
        HashSet<E> addedSet = new HashSet<>(added);

        // if we throw InterruptedException here it is
        // fine cause we haven't added anything to conditions or to a set
        mutex.acquire();
        Waiter waiter = new Waiter(removedSet.size() - howManyElemInSet(removedSet), waitersNum++);
        addRemoveConditions(removedSet, waiter);
        logDebug("Added conditions for " + Thread.currentThread().toString());
        mutex.release();


        try {
            waiter.mutex.acquire();
            logDebug("Invoked " + Thread.currentThread().toString());
            mutex.acquire();
        } catch (InterruptedException e) {
            // in this case we need to remove conditions
            removeConditions(waiter);
            throw e;
        }

        if (Thread.currentThread().isInterrupted()) {
            // same goes for this snippet of code
            removeConditions(waiter);
            throw new InterruptedException();
        }

        logDebug("Actually adding in " + Thread.currentThread().toString());
        removeConditions(waiter);
        elements.removeAll(removedSet);
        changeConditions(removedSet, 1);
        elements.addAll(addedSet);
        changeConditions(addedSet, -1);
        logDebug("Changed conditions");

        for (List<Waiter> waiters: elementsAvailability.values()) {
            boolean alreadyInvoked = false;

            for (Waiter waiter_aux: waiters) {
                if (waiter_aux.elemNumber == 0 && !waiter_aux.wasInvoked) {
                    logDebug("Invoking waiter");
                    waiter_aux.invoke();
                    alreadyInvoked = true;
                    break;
                }
            }

            if (alreadyInvoked) {
                break;
            }
        }

        mutex.release();
        logDebug("Elements in a set " + elements.size() + " in a " + Thread.currentThread().toString());
    }

    private static class Waiter {
        int elemNumber;
        Semaphore mutex;
        private boolean wasUp;
        private int num;
        boolean wasInvoked = false;

        Waiter(int elems, int index) {
            this.elemNumber = elems;
            this.mutex = elems == 0 ? new Semaphore(1) : new Semaphore(0);
            this.wasUp = elems == 0;
            this.num = index;
        }

        void addToElems(int k) {
            elemNumber += k;

            if (elemNumber != 0 && wasUp) {
                wasUp = false;
                mutex = new Semaphore(0);
            }
        }

        private void invoke() {
            this.mutex.release();
            wasInvoked = true;
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