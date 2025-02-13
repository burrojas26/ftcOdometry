import java.time.LocalDateTime; // Import the LocalDateTime class
import java.time.format.DateTimeFormatter; // Import the DateTimeFormatter class
import java.io.File;  // Import the File class
import java.io.IOException; 
import java.io.FileWriter; // Import the IOException class to handle errors
public class test {
    public static void main(String[] args) {
        LocalDateTime time = LocalDateTime.now();
        DateTimeFormatter format = DateTimeFormatter.ofPattern("dd-MM-yyyy_HH:mm");
        String formattedDate = time.format(format);
        System.out.println(formattedDate);
        try {
            FileWriter writer = new FileWriter("data/05-02-2025-10-33.txt");
            for(int i = 0; i < 100; i++) {
                time = LocalDateTime.now();
                format = DateTimeFormatter.ofPattern("dd, MM, yyyy, HH, mm, ss");
                formattedDate = time.format(format);
                writer.write(formattedDate + ", " + "0, 0, 0, 0, 0, 0\n");
                try {
                    Thread.sleep(2000);
                } catch(InterruptedException e) {
                    System.out.println("Error");
                }
            }
            writer.close();
            
          } catch (IOException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
          }
    }
}

