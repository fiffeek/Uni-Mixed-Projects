package solutions;

import swapper.Swapper;

import java.util.*;
import java.util.concurrent.Semaphore;

public class ProducenciKonsumenci {

    private static final int SIZE = 5;
    private static final int[] bufor = new int[SIZE];
    private static final Integer mutex = 1;
    private static final Integer canConsume = 2;
    private static final Integer canProduce = 3;
    private static final Swapper<Integer> GlobalSwap = new Swapper<>();
    private static Collection<Integer> globalEmpty = Collections.emptyList();
    private static int firstToConsume = 0;
    private static int firstToPlace = 0;
    private static int produced = 0;


    private static class Producent implements Runnable {

        int valueToPut;
        private final Random rd = new Random();

        public Producent() {}

        @Override
        public void run() {
            while (true) {
                try {
                    GlobalSwap.swap(Arrays.asList(mutex, canProduce), globalEmpty);
                    this.valueToPut = rd.nextInt(100);
                    produced++;
                    bufor[firstToPlace] = valueToPut;
                    System.out.println("Produced " + valueToPut + " on " + firstToPlace);
                    firstToPlace = (firstToPlace + 1) % SIZE;

                    if (produced < SIZE && firstToPlace != firstToConsume) {
                        GlobalSwap.swap(globalEmpty, Arrays.asList(canProduce));

                        if (produced > 0) {
                            GlobalSwap.swap(globalEmpty, Arrays.asList(canConsume));
                        }

                        GlobalSwap.swap(globalEmpty, Arrays.asList(mutex));
                    } else {
                        GlobalSwap.swap(globalEmpty, Arrays.asList(mutex, canConsume));
                    }
                } catch (Exception e) {

                }
            }
        }
    }

    private static class Konsument implements Runnable {

        @Override
        public void run() {
            while (true) {
                try {
                    GlobalSwap.swap(Arrays.asList(mutex, canConsume), globalEmpty);
                    int consumed = bufor[firstToConsume];
                    produced--;
                    System.out.println("Eaten " + consumed + " on " + firstToConsume);
                    firstToConsume = (firstToConsume + 1) % SIZE;

                    if (produced > 0) {
                        GlobalSwap.swap(globalEmpty, Arrays.asList(canConsume));

                        if (produced < SIZE && firstToConsume != firstToPlace) {
                            GlobalSwap.swap(globalEmpty, Arrays.asList(canProduce));
                        }

                        GlobalSwap.swap(globalEmpty, Arrays.asList(mutex));
                    } else {
                        GlobalSwap.swap(globalEmpty, Arrays.asList(canProduce, mutex));
                    }
                } catch (Exception e) {

                }
            }
        }

    }

    public static void main(String args[]) throws Exception {
        GlobalSwap.swap(globalEmpty, Arrays.asList(mutex, canProduce));


        Thread t1 = new Thread(new Producent());
        Thread t3 = new Thread(new Producent());
        Thread t4 = new Thread(new Producent());
        Thread t2 = new Thread(new Konsument());

        t1.start();
        t3.start();
        t4.start();
        t2.start();

        t1.join();
        t3.join();
        t4.join();
        t2.join();
    }

}

















