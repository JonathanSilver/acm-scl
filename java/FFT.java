class Complex {

    double real;
    double imag;

    Complex() {
        this.real = 0;
        this.imag = 0;
    }

    Complex(double real, double imag) {
        this.real = real;
        this.imag = imag;
    }

    Complex add(Complex o) {
        return new Complex(real + o.real, imag + o.imag);
    }

    Complex subtract(Complex o) {
        return new Complex(real - o.real, imag - o.imag);
    }

    Complex multiply(Complex o) {
        return new Complex(real * o.real - imag * o.imag, real * o.imag + imag * o.real);
    }

    Complex divide(double d) {
        return new Complex(real / d, imag / d);
    }

    @Override
    public String toString() {
        return String.format("%.4f%s%.4fj", real, imag >= 0 ? "+" : "", imag);
    }
}

public class FFT {

    private static int revBit(int idx, int n) {
        int r = 0;
        for (int i = 0; (1 << i) < n; i++) {
            r <<= 1;
            r |= (idx >> i) & 1;
        }
        return r;
    }

    public static void fft(Complex[] a, int op) {
        for (int i = 0; i < a.length; i++) {
            if (i < revBit(i, a.length))
                Utils.swap(a, i, revBit(i, a.length));
        }
        for (int m = 2, k = 1; m <= a.length; m <<= 1, k <<= 1) {
            Complex wm = new Complex(Math.cos(2 * op * Math.PI / m),
                    Math.sin(2 * op * Math.PI / m));
            for (int i = 0; i < a.length; i += m) {
                Complex w = new Complex(1, 0);
                for (int j = 0; j < k; j++) {
                    Complex u = a[i + j];
                    Complex v = w.multiply(a[i + k + j]);
                    a[i + j] = u.add(v);
                    a[i + k + j] = u.subtract(v);
                    w = w.multiply(wm);
                }
            }
        }
        if (op == -1) {
            for (int i = 0; i < a.length; i++)
                a[i] = a[i].divide(a.length);
        }
    }
}