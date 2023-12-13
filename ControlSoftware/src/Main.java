import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.*;
import java.net.Socket;

public class Main {

    String order = "\r";
    static int angle = 0;
    static String message= "\r";

    public static void main(String[] args) {
        Main mainClass = new Main();

        String ip_address = "192.168.4.1";
        int port = 23;

        try {
            // Socket erstellen und Verbindung herstellen
            Socket socket = new Socket(ip_address, port);

            // Reader und Writer für die Socket-Verbindung erstellen
            BufferedReader reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));

            // Reader für Keyboard-Input
            BufferedReader readerKeyboard = new BufferedReader(new InputStreamReader(System.in));

            // Receiver: Endlosschleife zum Lesen von Daten
            new Thread(() -> {
                try {
                    String line;
                    while (true) {
                        line = reader.readLine();
                        System.out.println(line);
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }).start();

            // Sender: Daten senden (in diesem Fall "Hello")
            while(true) {
                try {
                    //part for reading keyboard input
                    char input;
                    input = (char) readerKeyboard.read();
                    message = input + angle + "\r";
                    System.out.println("Eingetippter angle: " + angle + "\n");


                    //part for writing over wifi
                    Thread.sleep(1000);
                    writer.write(mainClass.message);
                    System.out.println("Sended");
                    writer.flush();


                } catch (IOException  | InterruptedException e) {
                    e.printStackTrace();
                }

            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public Main() {
        JFrame frame = new JFrame("BotGUI");
        frame.setSize(300,200);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        JButton buttonUp = new JButton("⬆️");
        JButton buttonDown = new JButton("⬇️️");
        JButton buttonRight = new JButton("➡️");
        JButton buttonLeft = new JButton("⬅️️");
        JButton fire = new JButton("Fire");
        JSlider accelerate = new JSlider(JSlider.HORIZONTAL, 0, 4, 0);

        JPanel panel = new JPanel();

        panel.add(buttonUp);
        panel.add(buttonDown);
        panel.add(buttonLeft);
        panel.add(buttonRight);
        panel.add(fire);
        panel.add(accelerate);

        frame.add(panel);

        // ActionListener für die JButton-Elemente hinzufügen
        buttonUp.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                message = "up" + angle +"\r";
                System.out.println("Button Up wurde gedrückt");
            }
        });

        buttonDown.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                message = "down;" + angle + "\r";
                System.out.println("Button Down wurde gedrückt");
            }
        });

        buttonLeft.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                message = "left;" + angle + "\r";
                System.out.println("Button Left wurde gedrückt");
            }
        });

        buttonRight.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                message = "right\r";
                System.out.println("Button Right wurde gedrückt");
            }
        });

        fire.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                System.out.println("On/Off Button wurde gedrückt");
            }
        });

        /*accelerate.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                System.out.println("On/Off Button wurde gedrückt");
            }
        });*/



        // ChangeListener für den JSlider hinzufügen
        accelerate.addChangeListener(new ChangeListener() {
            @Override
            public void stateChanged(ChangeEvent e) {
                JSlider source = (JSlider) e.getSource();
                if (!source.getValueIsAdjusting()) {
                    int value = source.getValue();

                    System.out.println("Slider wurde auf " + value + " gesetzt");
                }
            }
        });

        frame.setVisible(true);
    }
}
