package solutions;

import swapper.Swapper;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.Random;

public class CzytelnicyPisarze {

    private static final Swapper<Integer> GlobalSwap = new Swapper<>();
    private static final int mutex = 0; // mutex
    private static final int globalVariable = 3; // access to global variables
    private static final int canWrite = 2; // access to write
    private static int howManyReading = 0;
    private static Collection<Integer> globalEmpty = Collections.emptyList();
    private static int MAX_READ_PER_THREAD = 8;
    private static int MAX_WRITES_PER_THREAD = 2;
    private static boolean accessTaken = false;

    private static class Czytelnik implements Runnable {

        private final Random rd = new Random();
        private static int position = 0;

        public Czytelnik() {
            position++;
        }

        @Override
        public void run() {
            for (int i = 0; i < MAX_READ_PER_THREAD; ++i) {
                try {

                    GlobalSwap.swap(Arrays.asList(globalVariable, mutex), globalEmpty);
                    //System.out.println("Started reading");
                    howManyReading++;

                    if (!accessTaken) {
                        GlobalSwap.swap(Arrays.asList(canWrite), globalEmpty);
                        accessTaken = true;
                    }

                    GlobalSwap.swap(globalEmpty, Arrays.asList(globalVariable, mutex));

                    Thread.sleep(rd.nextInt(100));
                    System.out.println("Reading from " + position);

                    GlobalSwap.swap(Arrays.asList(globalVariable), globalEmpty);
                    //System.out.println("End of reading");
                    howManyReading--;

                    if (howManyReading == 0) {
                        GlobalSwap.swap(globalEmpty, Arrays.asList(globalVariable, canWrite));
                        accessTaken = false;
                    } else {
                        GlobalSwap.swap(globalEmpty, Arrays.asList(globalVariable));
                    }

                } catch (Exception e) {
                    System.err.println("Thread interrupted");
                }
            }
        }
    }

    private static class Pisarz implements Runnable {

        private final Random rd = new Random();
        private static int position = 0;

        public Pisarz() {
            position++;
        }

        @Override
        public void run() {
            for (int i = 0; i < MAX_WRITES_PER_THREAD; ++i) {
                try {
                    GlobalSwap.swap(Arrays.asList(mutex, canWrite), globalEmpty);

                    //System.out.println("Started writing");
                    Thread.sleep(rd.nextInt(100));
                    System.out.println("Writing from " + position);
                    //System.out.println("End of writing");

                    GlobalSwap.swap(Arrays.asList(globalVariable), globalEmpty);
                    if (howManyReading == 0) GlobalSwap.swap(globalEmpty, Arrays.asList(canWrite));
                    GlobalSwap.swap(globalEmpty, Arrays.asList(globalVariable, mutex));

                } catch (Exception e) {
                    System.err.println("Thread interrupted");
                }
            }
        }
    }

    public static void main(String args[]) throws Exception {
        GlobalSwap.swap(globalEmpty, Arrays.asList(mutex, canWrite, globalVariable));

        Thread t2 = new Thread(new Pisarz());
        Thread t3 = new Thread(new Czytelnik());
        Thread t1 = new Thread(new Czytelnik());

        t2.start();
        t3.start();
        t1.start();
        t1.join();
        t2.join();
        t3.join();

    }

}
