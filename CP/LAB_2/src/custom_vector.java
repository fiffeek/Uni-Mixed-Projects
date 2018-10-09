import java.util.ArrayList;
import java.util.Arrays;

public class custom_vector {
    private int[] arr;

    custom_vector(int[] t) {
        arr = t;
    }

    int returnSize() {
        return arr.length;
    }

    boolean canMakeOperations(custom_vector aux) {
        return aux.returnSize() == this.returnSize();
    }

    int returnIth(int i) {
        return arr[i];
    }

    custom_vector addInBetween(int start, int end, custom_vector aux) {
        int[] al = new int[aux.returnSize()];

        for (int i = start; i < end; ++i) {
            al[i] = returnIth(i) + aux.returnIth(i);
        }

        return new custom_vector(al);
    }

    custom_vector multiplyInBetween(int start, int end, custom_vector aux) {
        int[] number = new int[1];
        number[0] = 0;

        for (int i = start; i < end; ++i) {
            number[0] += this.returnIth(i) * aux.returnIth(i);
        }

        return new custom_vector(number);
    }

    void agreageteMultiplication(custom_vector aux) {
        this.arr[0] += aux.returnIth(0);
    }

    void copyHere(int start, int end, custom_vector copyFrom) {
        for (int i = start; i < end; ++i) {
            this.arr[i] = copyFrom.returnIth(i);
        }
    }

    @Override
    public String toString() {
        return Arrays.toString(arr);
    }
}
