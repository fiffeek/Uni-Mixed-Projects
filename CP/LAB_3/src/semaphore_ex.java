import java.util.ArrayList;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.Semaphore;
import java.util.function.IntBinaryOperator;

public class semaphore_ex {

    private static final int WIERSZE = 10;
    private static final int KOLUMNY = 100;
    private static volatile int[] current_sum = new int[KOLUMNY];
    private static final CyclicBarrier sum_barier = new CyclicBarrier(KOLUMNY, new wypisanie());

    private static void wypisz(int wiersz) {
        int sum = 0;
        for (int i = 0; i < current_sum.length; ++i) {
            sum += current_sum[i];
        }
        System.out.println(wiersz + " " + sum);
    }
    private static class wypisanie implements Runnable {

        private static int row = 0;

        @Override
        public void run() {
            wypisz(row++);
        }
    }

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
                    current_sum[this.kolumna] = macierz.applyAsInt(this.wiersz, this.kolumna);
                    sum_barier.await();

                    this.wiersz++;
                } catch (Exception e) {

                }
            }
        }
    }

    private static void piszSumyWierszy(int wiersze, int kolumny, IntBinaryOperator macierz) {
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