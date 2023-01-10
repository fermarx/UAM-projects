package main;

import race.Race;
import race.RaceException;
import race.RaceReader;

import java.io.IOException;

public class mainEx2 {
    public static void main(String [] args) {
        Race r;
        try {
            r = RaceReader.read(args[0]);
            r.simulate();
        } catch (IOException e) {
            System.out.println("Error reading the file");
        } catch (RaceException e) {
            System.out.println(e);
        }
    }

}
