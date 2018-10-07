import java.util.Arrays;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.stream.IntStream;

public class cp_prime {

    public static void main(String[] args) {
        AtomicInteger howManyPrimes = new AtomicInteger(0);
        IntStream.range(2, 10000 + 1).forEach(
                i -> {
                    if (isPrime(i)) howManyPrimes.getAndIncrement();
                }
        );

        System.out.print(howManyPrimes);
    }

    private static boolean isPrime(int numberToCheck) {
        int[] startersArray = new int[]{31, 37, 41, 43, 47, 49, 53, 59};
        int[] auxArray = new int[]{2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
        threadRun.shouldRun = true;

        if (check(numberToCheck, auxArray)) {
            Thread[] openThreads = new Thread[8];

            for (int i = 0; i < 8; i++) {
                Thread t = new Thread(
                        new threadRun(startersArray[i], numberToCheck)
                );

                openThreads[i] = t;

                t.start();
            }

            while (Arrays.stream(openThreads).parallel().anyMatch(Thread::isAlive)) {}

            return threadRun.shouldRun;
        } else {
            return IntStream.of(auxArray).anyMatch(x -> x == numberToCheck);
        }
    }

    public static boolean check(int n, int[] auxArray) {

        for (int aux: auxArray) {
            if (n % aux == 0) return false;
        }

        return true;
    }

    private static class threadRun implements Runnable {

        private int starter;
        private int toCheck;
        private static volatile boolean shouldRun = true;

        public threadRun(int starter, int toCheck) {
            this.starter = starter;
            this.toCheck = toCheck;
        }

        @Override
        public void run() {
            if (!threadRun.shouldRun) return;

            while (this.starter * this.starter <= this.toCheck) {

                if (this.toCheck % this.starter == 0) {
                    threadRun.shouldRun = false;
                    return;
                }

                this.starter += 30;
            }
        }
    }
}
