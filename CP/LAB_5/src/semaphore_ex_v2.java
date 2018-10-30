import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.function.IntBinaryOperator;

public class semaphore_ex_v2 {

    private static final int WIERSZE = 10;
    private static final int KOLUMNY = 100;

    private static class threadRun implements Callable<Integer> {

        private int wiersz, kolumna;
        private IntBinaryOperator macierz;

        threadRun(int wiersz, int kolumna, IntBinaryOperator macierz) {
            this.wiersz = wiersz;
            this.kolumna = kolumna;
            this.macierz = macierz;
        }

        @Override
        public Integer call() throws InterruptedException {
            return macierz.applyAsInt(this.wiersz, this.kolumna);
        }
    }

    private static void piszSumyWierszy(int wiersze, int kolumny, IntBinaryOperator macierz) {
        ExecutorService pula = Executors.newFixedThreadPool(4);

        try {
            for (int w = 0; w < WIERSZE; ++w) {
                List<Callable<Integer>> obliczenia = new ArrayList<>();

                for (int k = 0; k < kolumny; ++k) {
                    obliczenia.add(new threadRun(w, k, macierz));
                }

                List<Future<Integer>> obietnice = pula.invokeAll(obliczenia);
                int sum = 0;

                for (Future<Integer> kolejna : obietnice) {
                    sum += kolejna.get();
                }

                System.out.println(sum);
            }
        } catch (Exception e) {

        } finally {
            pula.shutdown();
        }
    }

    public static void main(String[] args) {
        piszSumyWierszy(WIERSZE, KOLUMNY, (wiersz, kolumna) -> {
            int a = 2 * kolumna + 1;
            return (wiersz + 1) * (a % 4 - 2) * a;
        });
    }

}