package swapper;
import java.util.*;

public class SwapperRef<E> {

    private HashSet<E> set = new HashSet<>();

    public synchronized void swap(Collection<E> removed, Collection<E> added) throws InterruptedException {
        while (!set.containsAll(removed))
            wait();
        set.removeAll(removed);
        set.addAll(added);
        notifyAll();
    }

    @Override
    public String toString() {
        return set.toString();
    }
}