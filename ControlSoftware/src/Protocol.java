import java.io.*;

public class Protocol implements BotInterface{
    String message, answer, m_direction, m_speed, gunPos = "\r";
    int ready, fire = 0;
    BufferedReader reader;
    BufferedWriter writer;

    public Protocol(BufferedReader reader, BufferedWriter writer) {
        /**** Receiver: Thread zum Lesen von Daten
         * Protokollform:
         * Attribution:
         */
        this.writer = writer;
        this.reader = reader;

        new Thread(() -> {
            while(true){
                try {
                    answer = reader.readLine();
                    /*if (answer != null) {
                        System.out.println("read");
                        System.out.println(answer);
                    }*/
                    System.out.println(answer);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }).start();


        /**** SENDER: Thread zum Schreiben von Daten
         * Protokollform:   "m_direction;m_speed;fire;ready;gunPos"
         * Attribution:     "String;String;String;String;String"
         *
        new Thread(() -> {
            try {
                while (true) {
                    message = m_direction + ";" + m_speed + ";" + fire + ";" + ready + ";" + gunPos + "\r";
                    Thread.sleep(1000);
                    writer.write(message);
                    writer.flush();
                }
            } catch (IOException  | InterruptedException e) {
                e.printStackTrace();
            }
        }).start();*/
    }

    public void send(){
        try {
            message = m_direction + ";" + m_speed + ";" + fire + ";" + ";" + gunPos + "\r";
            writer.write(message);
            System.out.println("Sent");
            writer.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void printAnswer(){ System.out.println("Answer: " + answer + "\n"); };


    public void setM_direction(String m_direction) {
        this.m_direction = m_direction;
    }

    public void setM_speed(String m_speed) {
        this.m_speed = m_speed;
    }

    public void setGunPos(String gunPos){
        this.gunPos = gunPos;
    }

    @Override
    public void driveBackwards() { drive(-1); }

    @Override
    public void driveForwards() { drive(1); }

    @Override
    public void turnLeft() { drive(-2); }

    @Override
    public void turnRight() { drive(2); }

    @Override
    public void setReady(boolean rdy) {
        if (rdy)
            this.ready = 1;
        else
            this.ready = 0;
    }

    public boolean getReady(){
        if (this.ready == 1)
            return true;
        else
            return false;
    }

    @Override
    public void fire() { fire = 1; }

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
