import java.util.*;
import java.io.*;
import java.math.*;

public class Main {
	public static void main(String[] args) {
		init();
		Integer x;
		while ((x = nextInt()) != null) {
			System.out.println(x);
		}
	}
	
	public static BufferedReader reader;
	public static StringTokenizer tokenizer;
	public static void init() {
		reader = new BufferedReader(new InputStreamReader(System.in), 32768);
		tokenizer = null;
	}
	public static String next() {
		while (tokenizer == null || !tokenizer.hasMoreTokens()) {
			try {
				tokenizer = new StringTokenizer(reader.readLine());
			} catch (Exception e) {
				return null;
			}
		}
		return tokenizer.nextToken();
	}
	public static String nextLine() {
		try {
			return reader.readLine();
		} catch (IOException e) {
			return null;
		}
	}
	public static Integer nextInt() {
		try {
			return Integer.valueOf(next());
		} catch (Exception e) {
			return null;
		}
	}
}