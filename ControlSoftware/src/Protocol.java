import java.io.*;

public class Protocol implements BotInterface{
    String message, answer, m_direction = "\r";
    //positionReady rausnehmen
    int gunPos, fire, m_speed, gunReady, positionReady = 0;
    BufferedReader reader;
    BufferedWriter writer;

    /**** Receiver: Thread zum Lesen von Daten
     * Protokollform:
     * Attribution:
     */
    public Protocol(BufferedReader reader, BufferedWriter writer) {

        this.writer = writer;
        this.reader = reader;


        new Thread(() -> {
            System.out.println("Receiving");
            while(true){
                String temp = "\n";

                try {
                    //System.out.println("Receiving1");
                    //System.out.println(reader.ready());
                    if (reader.ready())
                        temp = reader.readLine();
                    //System.out.println("Receiving2");
                    if (!temp.isBlank())
                        System.out.println("Received: " + temp);
                    //System.out.println("Receiving3");
                    Thread.sleep(500);
                    send();
                } catch (IOException | InterruptedException e) {
                    System.out.println("theres a problem in receiving");
                    e.printStackTrace();
                }
            }
        }).start();
    }

    /**** SENDER: Thread zum Schreiben von Daten
     * Protokollform:   "m_direction;m_speed;fire;gunPos"
     * Attribution:     "String;String;int;int"
     */
    public void send() {
        //message = "ab" + ";" + 1 + ";" + 2 + ";" + 3;
        try {
            message = m_direction + ";" + m_speed + ";" + fire + ";" + gunPos + ";" + "\r";
            System.out.println(message);
            //Thread.sleep(1000);
            writer.write(message);
            writer.flush();
            reset();
            //System.out.println("Sent");
            //writer.close();
        } catch (IOException e) {//| InterruptedException e) {
            //writer.close();
            e.printStackTrace();
        }
    }

    void reset(){
        this.m_direction = "0";
        this.fire = 0;
    }

    public void setM_direction(String m_direction) {
        this.m_direction = m_direction;
        //send();
    }

    public void setM_speed(int m_speed) {
        this.m_speed = m_speed;
        //send();
    }

    public void setGunPos(int gunPos){
        this.gunPos = gunPos;
        //send();
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
    public void setGunReady(boolean rdy) {
        if (rdy)
            this.gunReady = 1;
        else
            this.gunReady = 0;
    }

    public void setPositionReady(boolean rdy) {
        if (rdy)
            this.positionReady = 1;
        else
            this.positionReady = 0;
    }

    public boolean getGunReady(){
        return this.gunReady == 1;
    }

    /*public boolean getPositionReady(){
        return this.positionReady == 1;
    }*/
    @Override
    public void fire() {
        fire = 1;
        //send();
    }

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
