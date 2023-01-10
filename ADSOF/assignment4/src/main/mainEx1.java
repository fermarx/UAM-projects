package main;

import race.Race;
import race.RaceReader;

import java.io.IOException;
import race.RaceException;

public class mainEx1 {
    public static void main(String [] args) {
        Race r;
        try {
            r = RaceReader.read(args[0]);
            System.out.println(r);
        } catch (IOException e) {
            System.out.println("Error reading the file");
        } catch (RaceException e) {
            System.out.println(e);
        }
    }
}
