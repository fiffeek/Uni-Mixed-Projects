import java.util.ArrayList;

public class cp_vectors_operations {

    public static void main(String[] args) {
        int[] arr1 = new int[]{1, 2, 3, 4, 6, 1, 2, 3, 4, 6, 1, 2, 3, 4, 6};
        int[] arr2 = new int[]{2, 8, 12, 1, 303, 2, 8, 12, 1, 303, 2, 8, 12, 1, 303};
        custom_vector cv1 = new custom_vector(arr1);
        custom_vector cv2 = new custom_vector(arr2);

        if (cv1.canMakeOperations(cv2)) {
            custom_vector out = new custom_vector(new int[cv1.returnSize()]);
            ArrayList<Thread> aux_threads = new ArrayList<>();

            for (int i = 0; i < cv1.returnSize(); i = i + 10) {
                Thread aux = new Thread(
                        new makeOperationsSum(
                                i,
                                Math.min(i + 10, cv1.returnSize()),
                                cv1,
                                cv2,
                                out
                                ));

                aux_threads.add(aux);
                aux.start();
            }


            try {
                for (Thread t: aux_threads) {
                    t.join();
                }

                System.out.println(out);
            } catch (InterruptedException e) {
                System.out.println("Interrupted.");
            }
        } else {
            System.out.println("Operation is not permitted.");
        }
    }

    private static class makeOperationsSum implements Runnable {

        private int start, end;
        private custom_vector v1, v2, out;

        makeOperationsSum(int start, int end, custom_vector v1, custom_vector v2, custom_vector out) {
            this.start = start;
            this.end = end;
            this.v1 = v1;
            this.v2 = v2;
            this.out = out;
        }

        @Override
        public void run() {
            custom_vector aux = v1.addInBetween(start, end, v2);
            out.copyHere(start, end, aux);
        }
    }

    private static class makeOperationsMultiply implements Runnable {

        private int start, end;
        private custom_vector v1, v2;
        private volatile custom_vector out;

        makeOperationsMultiply(int start, int end, custom_vector v1, custom_vector v2, custom_vector out) {
            this.start = start;
            this.end = end;
            this.v1 = v1;
            this.v2 = v2;
            this.out = out;
        }

        @Override
        public void run() {
            custom_vector aux = v1.multiplyInBetween(start, end, v2);
            out.agreageteMultiplication(aux);
        }
    }
}
