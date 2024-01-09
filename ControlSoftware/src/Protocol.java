import java.io.*;

public class Protocol implements BotInterface{
    String message, answer, m_direction, m_speed = "\r";
    int angle = 0;

    public Protocol(BufferedReader reader, BufferedWriter writer) {
        // Receiver: Endlosschleife zum Lesen von Daten
        new Thread(() -> {
            try {
                while (true) {
                    answer = reader.readLine();
                    System.out.println(answer);
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }).start();


        //SENDER
        new Thread(() -> {
            try {
                while (true) {
                    message = m_direction + ";" + m_speed + "\r";
                    Thread.sleep(1000);
                    writer.write(message);
                    writer.flush();
                }
            } catch (IOException  | InterruptedException e) {
                e.printStackTrace();
            }
        }).start();
    }

    public void setM_direction(String m_direction) {
        this.m_direction = m_direction;
    }

    public void setM_speed(String m_speed) {
        this.m_speed = m_speed;
    }

    @Override
    public void driveBackwards() { drive(-1); }

    @Override
    public void driveForwards() { drive(1); }

    @Override
    public void turnLeft() { drive(-2); }

    @Override
    public void turnRight() { drive(2); }

    public void drive(int sp){
        if (sp == 1)
            setM_direction("w");
        if (sp == -1)
            setM_direction("s");
        if (sp == -2)
            setM_direction("a");
        if (sp == 2)
            setM_direction("d");
    }

}
