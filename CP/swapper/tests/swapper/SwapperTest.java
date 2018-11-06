package swapper;

import jdk.nashorn.internal.runtime.ECMAException;
import org.junit.Test;

import java.util.*;

public class SwapperTest {

    private static class ThreadGiver implements Runnable {
        private Swapper<Integer> machine;
        private Collection<Integer> add;
        private Collection<Integer> remove;

        ThreadGiver(Swapper<Integer> s, Collection<Integer> add, Collection<Integer> rem) {
            this.machine = s;
            this.add = add;
            this.remove  = rem;
        }

        @Override
        public void run() {
            try {
                machine.swap(add, remove);
            } catch (Exception e) {
               System.err.println("EXCEPTION HANDLED");
            }
        }
    }


    @org.junit.Test
    public void swapConcurrentTest() throws Exception {
        Swapper<Integer> s = new Swapper<>();
        HashSet<Integer> empty = new HashSet<>();
        HashSet<Integer> onethree = new HashSet<>(Arrays.asList(1, 3));
        HashSet<Integer> onetwo = new HashSet<>(Arrays.asList(1, 2));
        HashSet<Integer> three = new HashSet<>(Arrays.asList(3));

//        Thread t1 = new Thread(new ThreadGiver(s, empty, onetwothree), "[] -> [1, 2, 3]");
//        Thread t2 = new Thread(new ThreadGiver(s, onetwo, empty), "[1, 2] -> []");
//        Thread t3 = new Thread(new ThreadGiver(s, twothree, empty), "[2, 3] -> []");
//
//        t3.start();
//        t1.start();
//        t2.start();
//        t2.interrupt();
//        t3.join();
//        t1.join();
    }


    @org.junit.Test
    public void swap() throws Exception {
        Swapper<Integer> s = new Swapper<>();
        HashSet<Integer> empty = new HashSet<>();
        HashSet<Integer> one = new HashSet<>(Arrays.asList(1));
        HashSet<Integer> two = new HashSet<>(Arrays.asList(2));
        HashSet<Integer> oneTwo = new HashSet<>(Arrays.asList(1, 2));
        HashSet<Integer> somewhat = new HashSet<>(Arrays.asList(1, 0, 0, 0, 2, 3, 5));

        Thread t1 = new Thread(new ThreadGiver(s, empty, one), "[] -> [1]");
        Thread t2 = new Thread(new ThreadGiver(s, oneTwo, somewhat), "[1, 2] -> [1, 0, 0, 0, 2, 3, 5]");
        Thread t3 = new Thread(new ThreadGiver(s, one, two), "[1] -> [2]");
        Thread t4 = new Thread(new ThreadGiver(s, empty, two), "[] -> [2]");
        Thread t5 = new Thread(new ThreadGiver(s, empty, one), "[] -> [1]");

        t1.start();
        t1.join();
        t3.start();
        t3.join();
        t4.start();
        t4.join();
        t2.start();
        t5.start();
        t2.join();
        t5.join();
    }

    @org.junit.Test
    public void swapWithInterrupt() throws Exception {
        Swapper<Integer> s = new Swapper<>();
        HashSet<Integer> one = new HashSet<>(Arrays.asList(1, 4, 5));
        HashSet<Integer> two = new HashSet<>(Arrays.asList());
        HashSet<Integer> three = new HashSet<>(Arrays.asList(1, 4));

        Thread waiter = new Thread(new ThreadGiver(s, one, one), "[1, 4, 5] -> [1, 4, 5]");
        Thread go = new Thread(new ThreadGiver(s, two, three), "[] -> [1, 4]");
        waiter.start();
        go.start();
        go.join();


    }

    @Test
    public void moodleValidator() {
        Swapper<Integer> swapper = new Swapper<>();
        try {
            Collection<Integer> empty = Collections.emptySet();
            Collection<Integer> singletonOne = Collections.singleton(1);
            swapper.swap(empty, singletonOne);

            swapper.swap(singletonOne, singletonOne);
            assert(true);
        } catch (InterruptedException e) {
            assert(false);
        }
    }
}
