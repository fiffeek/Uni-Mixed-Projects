import java.util.ArrayList;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.Semaphore;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.function.IntBinaryOperator;

public class semaphore_ex_v2 {

    private static final int WIERSZE = 10;
    private static final int KOLUMNY = 100;
    private static volatile AtomicInteger[] current_sum = new AtomicInteger[WIERSZE];
    private static ConcurrentHashMap<Integer, AtomicInteger> hm = new ConcurrentHashMap<>();
    private static ConcurrentHashMap<Integer, AtomicInteger> ai = new ConcurrentHashMap<>();
    private static int actual_to_write = 0;
    private static Semaphore mutex = new Semaphore(1);

    private static class threadRun implements Runnable {

        private int wiersz, kolumna;
        private IntBinaryOperator macierz;

        threadRun(int wiersz, int kolumna, IntBinaryOperator macierz) {
            this.wiersz = wiersz;
            this.kolumna = kolumna;
            this.macierz = macierz;
        }

        @Override
        public void run() {
            while (this.wiersz < WIERSZE) {
                try {
                    hm.get(this.wiersz).getAndAdd(macierz.applyAsInt(this.wiersz, this.kolumna));
                    ai.get(this.wiersz).getAndDecrement();

                    mutex.acquire();
                    if (actual_to_write == this.wiersz && ai.get(this.wiersz).get() == 0) {
                        System.out.println(hm.get(this.wiersz).get());
                        hm.remove(this.wiersz);
                        ai.remove(this.wiersz);
                        actual_to_write++;
                    }
                    mutex.release();

                    this.wiersz++;
                } catch (Exception e) {

                }
            }
        }
    }

    private static void piszSumyWierszy(int wiersze, int kolumny, IntBinaryOperator macierz) {

        for (int i = 0; i < WIERSZE; ++i) {
            hm.put(i, new AtomicInteger(0));
            ai.put(i, new AtomicInteger(KOLUMNY));
        }

        ArrayList<Thread> al = new ArrayList<>();

        for (int k = 0; k < kolumny; ++k) {
            Thread t = new Thread(new threadRun(0, k, macierz));
            al.add(t);
            t.start();
        }

        for (Thread t: al) {
            try {
                t.join();
            } catch (Exception e) {

            }
        }
    }

    public static void main(String[] args) {
        piszSumyWierszy(WIERSZE, KOLUMNY, (wiersz, kolumna) -> {
            int a = 2 * kolumna + 1;
            return (wiersz + 1) * (a % 4 - 2) * a;
        });
    }

}