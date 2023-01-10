package pr3.fines;

import java.io.*;
import java.util.ArrayList;
import java.util.List;

public abstract class FineReader{
    public static List<Fine> read(String filename) {
        List<Fine> fines = new ArrayList<>();
        FileInputStream stream = null;
		try {
			stream = new FileInputStream(filename);
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
        InputStreamReader reader = new InputStreamReader(stream);
        BufferedReader buffer = new BufferedReader(reader);

        String line;
        try {
			while((line = buffer.readLine()) != null){
                fines.add(new Fine(line));
			}
		} catch (IOException e) {
			e.printStackTrace();
		}

        return fines;
    }
}
