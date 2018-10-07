import java.util.Random;

public class cp_first {

    private static final int n = 5;

    public static void main(String[] args) {
        newThreadRunner ntr = new newThreadRunner(n);
        ntr.run();
    }

    private static class newThreadRunner implements Runnable {

        private int t;

        newThreadRunner(int t) {
            this.t = t;
        }

        @Override
        public void run() {
            if (this.t > 1) {
                Thread newThread = new Thread(
                        new newThreadRunner(this.t - 1),
                        "" + (this.t - 1));
                newThread.start();
            }

            System.out.println(this.t);
            doSmth();
        }

        public void doSmth() {
            Random rd = new Random(10 * this.t);

            try {
                long toSleep = rd.nextInt(10000);

                System.out.println(Thread.currentThread());

                Thread.currentThread().sleep(toSleep);
                System.out.println(toSleep);
            } catch (Exception e) {

            }
        }
    }
}
