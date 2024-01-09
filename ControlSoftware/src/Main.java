import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.io.*;
import java.net.Socket;

public class Main {

    String order = "\r";
    static int angle = 0;
    static String message= "\r";
    static Protocol prot;

    public static void main(String[] args) {
        Main mainClass = new Main();

        String ip_address = "192.168.4.1";
        int port = 23;

        try {
            Socket socket = new Socket(ip_address, port);

            // Reader und Writer für die Socket-Verbindung erstellen
            BufferedReader reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
            prot = new Protocol(reader, writer);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public Main() {
        JFrame frame = new JFrame("BotGUI");
        frame.setSize(300,200);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);


        JButton fire = new JButton("Fire");
        JSlider accelerate = new JSlider(JSlider.HORIZONTAL, 0, 4, 0);

        JPanel panel = new JPanel();

        panel.add(fire);
        panel.add(accelerate);
        frame.add(panel);


        // ActionListener für die JButton-Elemente hinzufügen
        fire.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                System.out.println("On/Off Button wurde gedrückt");
            }
        });


        // ChangeListener für den JSlider hinzufügen
        accelerate.addChangeListener(new ChangeListener() {
            @Override
            public void stateChanged(ChangeEvent e) {
                int value = accelerate.getValue();
                prot.setM_speed(String.valueOf(value));
                frame.requestFocusInWindow();
            }
        });

        frame.addKeyListener(new KeyAdapter() {
            @Override
            public void keyTyped(KeyEvent e) {
                char input = e.getKeyChar();

                if(input == 'w')
                    prot.driveForwards();
                if(input == 's')
                    prot.driveBackwards();
                if(input == 'd')
                    prot.turnRight();
                if(input == 'a')
                    prot.turnLeft();
            }
        });


        frame.requestFocusInWindow();
        frame.setFocusable(true);

        frame.setVisible(true);
    }
}
