package swapper;
import java.util.*;

/**
 * Swapper reference, fast solution to check
 * whether the main swapper is implemented in a good way
 * @param <E>
 */
public class SwapperRef<E> {

    private HashSet<E> set = new HashSet<>();

    public synchronized void swap(Collection<E> removed, Collection<E> added) throws InterruptedException {
        while (!set.containsAll(removed)) {
            wait();
        }

        set.removeAll(removed);
        set.addAll(added);
        notifyAll();
    }
}