import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.net.Socket;

public class Main {

    //following objects server connecting
    Socket socket;
    static Protocol prot = null;
    static String ip_address = "192.168.4.1";
    static int port = 23;


    //following objects for GUI-generation
    JPanel mainPanel = new JPanel();
    JPanel leftMainPanel = new JPanel();
    JPanel rightMainPanel = new JPanel();
    JPanel informationPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
    JPanel shootPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
    JPanel speedPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
    JPanel gunPositionPanel = new JPanel(new BorderLayout());
    JPanel monitoringPanel = new JPanel(new BorderLayout());

    /****
     * JButton fire                     => Object 1
     * JButton ready                    => Object 2
     * JSlider speed                    => Object 3
     * JSlider gunPosition              => Object 4
     * JTextArea monitoringCommands     => Object 5
     * JLabel shoot                     => Object 6
     * JLabel setReady                  => Object 7
     * JScrollPane pane                 => Object 8
     * JLabel currentPosition           => Object 9
     * JLabel currentMovement           => Object 10
     */
    JButton fire = new JButton("Fire");
    JButton ready = new JButton("NotReadyToFire");
    //JSlider speed = new JSlider(JSlider.HORIZONTAL, 0, 50, 0);
    JSlider gunPosition = new JSlider(JSlider.HORIZONTAL, 0, 4, 0);
    JTextArea monitoringCommands = new JTextArea("");
    JLabel shoot = new JLabel("Shoot one dart!");
    JLabel setReady = new JLabel("Set ready!");
    JLabel currentPosition = new JLabel("Current Position: 0");
    JLabel currentMovement = new JLabel("Current Movement: -");
    JScrollPane scrollPane;

    /***
     * Dimension-Array containing every x and y size
     * respecting the index order of stated objects above
     * respecting the space between Object2 and Object 6 declared below (20 units space)
     *
     * for reference:   longest x-distance: 19*20 + 20 => 400
     *                  longest y-distance: 15*10 => 150
     */
    static int smallObjectsX = 20;//40
    static int smallObjectsY = 10;//20
    static Dimension[] dimensions = {
            new Dimension(smallObjectsX * 4, smallObjectsY * 3),
            new Dimension(smallObjectsX * 7, smallObjectsY * 3),
            new Dimension(smallObjectsX * 20, smallObjectsY * 2),
            new Dimension(smallObjectsX * 3, smallObjectsY * 18),       //only object on right side
            new Dimension(smallObjectsX * 21, smallObjectsY * 15),
            new Dimension(smallObjectsX * 5, smallObjectsY * 2),
            new Dimension(smallObjectsX * 3, smallObjectsY * 2),
            new Dimension(smallObjectsX * 7, smallObjectsY * 2),
            new Dimension(smallObjectsX * 7, smallObjectsY * 3)
    };

    static int borderDistance = 7;

    private static void appendText(JTextArea textArea, String text){
        textArea.append(text + "\n");
        textArea.setCaretPosition(textArea.getDocument().getLength());
    }


    public static void main(String[] args) {
        Main mainClass = new Main(ip_address, port);
    }

    public Main(String addr, int port) {
        //Setting up Socket connection with DjangoBot
        try {
            socket = new Socket(addr, port);
            // Reader und Writer für die Socket-Verbindung erstellen
            BufferedReader reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
            prot = new Protocol(reader, writer);
            System.out.println("Prot set!");
        } catch (IOException e) {
            System.out.println("Socketserver has to run!");
            System.exit(1);
            e.printStackTrace();
        }

        JFrame frame = new JFrame("DjangoGUI");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        //Object specific changes
        shoot.setOpaque(false);
        setReady.setOpaque(false);
        ready.setBackground(Color.RED);
        gunPosition.setOrientation(1);
        monitoringCommands.setEditable(false);
        scrollPane = new JScrollPane(monitoringCommands);
        scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);

        //setting preferred size-dependencies between objects using Int-Arrays
        fire.setPreferredSize(dimensions[0]);
        ready.setPreferredSize(dimensions[1]);
        //speed.setPreferredSize(dimensions[2]);
        gunPosition.setPreferredSize(dimensions[3]);
        monitoringCommands.setSize(dimensions[4]);
        shoot.setPreferredSize(dimensions[5]);
        setReady.setPreferredSize(dimensions[6]);
        currentPosition.setPreferredSize(dimensions[7]);
        currentMovement.setPreferredSize(dimensions[8]);


        //Setting Borders of each operating Panel
        gunPositionPanel.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createTitledBorder("Gun Position"),
                BorderFactory.createEmptyBorder(borderDistance,borderDistance,borderDistance,borderDistance)));
        shootPanel.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createTitledBorder("Shot"),
                BorderFactory.createEmptyBorder(borderDistance,borderDistance,borderDistance,borderDistance)));
        speedPanel.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createTitledBorder("Setting up dart drum"),
                BorderFactory.createEmptyBorder(borderDistance,borderDistance,borderDistance,borderDistance)));
        monitoringPanel.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createTitledBorder("Monitoring"),
                BorderFactory.createEmptyBorder(borderDistance,borderDistance,borderDistance,borderDistance)));
        informationPanel.setBorder(BorderFactory.createCompoundBorder(
                BorderFactory.createTitledBorder("Info"),
                BorderFactory.createEmptyBorder(borderDistance,borderDistance,borderDistance,borderDistance)));

        BoxLayout box1 = new BoxLayout(mainPanel, BoxLayout.X_AXIS);
        BoxLayout box2 = new BoxLayout(leftMainPanel, BoxLayout.Y_AXIS);
        BoxLayout box3 = new BoxLayout(rightMainPanel, BoxLayout.Y_AXIS);
        //BoxLayout box4 = new BoxLayout(informationPanel, BoxLayout.Y_AXIS);
        mainPanel.setLayout(box1);
        leftMainPanel.setLayout(box2);
        rightMainPanel.setLayout(box3);
        //informationPanel.setLayout(box4);

        //adding Objects to ShootPanel
        //speedPanel.add(speed);
        shootPanel.add(setReady);
        shootPanel.add(ready);
        shootPanel.add(Box.createHorizontalStrut(20));
        shootPanel.add(shoot);
        shootPanel.add(fire);

        //adding JSlider to GunPositionPanel
        gunPositionPanel.add(gunPosition, BorderLayout.NORTH);
        gunPositionPanel.add(currentPosition, BorderLayout.SOUTH);

        //adding Objects to InformationPanel
        informationPanel.add(currentMovement);

        //adding MonitoringTextField to MonitoringPanel
        monitoringPanel.add(scrollPane, BorderLayout.CENTER);

        //adding Objects to RightMainPanel
        rightMainPanel.add(gunPositionPanel);
        rightMainPanel.add(informationPanel);

        //adding Objects to LeftMainPanel
        leftMainPanel.add(monitoringPanel);
        leftMainPanel.add(speedPanel);
        leftMainPanel.add(shootPanel);

        //adding every partial Panel to MainPanel
        mainPanel.add(leftMainPanel);
        mainPanel.add(rightMainPanel);



        mainPanel.setBorder(BorderFactory.createEmptyBorder(5,5,5,5));

        frame.add(mainPanel);




        // ActionListener für die JButton-Elemente hinzufügen
        fire.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String text1 = "Shot fired!";
                String text2 = "You need to configure the Gun Positioning!";
                if (prot.getGunReady() && prot.getPositionReady()) {
                    appendText(monitoringCommands, text1);
                    JOptionPane.showMessageDialog(frame, "Shot fired!\nGun gonna be reloaded.");
                    prot.fire();
                } else {
                    appendText(monitoringCommands, text2);
                }
                frame.requestFocusInWindow();
                //monitoringCommands.setCaretPosition(monitoringCommands.getDocument().getLength());
            }
        });


        ready.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (ready.getBackground().equals(Color.RED)){
                    ready.setBackground(Color.GREEN);
                    String text = "Ready to Fire!";
                    ready.setText(text);
                    appendText(monitoringCommands, text);
                    prot.setGunReady(true);
                } else {
                    ready.setBackground(Color.RED);
                    String text = "NOT ready to Fire!";
                    ready.setText(text);
                    appendText(monitoringCommands, text);
                    prot.setGunReady(false);
                }
                frame.requestFocusInWindow();
            }
        });

        // ChangeListener für den JSlider hinzufügen
        /*speed.addChangeListener(new ChangeListener() {
            @Override
            public void stateChanged(ChangeEvent e) {
                int value = speed.getValue();
                currentMovement.setText("Current Position: " + value);
                prot.setM_speed(value);
                frame.requestFocusInWindow();
            }
        });*/

        gunPosition.addChangeListener(new ChangeListener() {
            @Override
            public void stateChanged(ChangeEvent e) {
                int value = gunPosition.getValue();
                prot.setPositionReady(value != 0);
                prot.setGunPos(value);
                frame.requestFocusInWindow();
            }
        });

        frame.addKeyListener(new KeyAdapter() {
            @Override
            public void keyTyped(KeyEvent e) {
                char input = e.getKeyChar();
                //WASD - moving direction
                if(input == 'w') {
                    currentMovement.setText("Current Movement: w");
                    prot.driveForwards();
                }
                if(input == 's') {
                    currentMovement.setText("Current Movement: s");
                    prot.driveBackwards();
                }
                if(input == 'd') {
                    currentMovement.setText("Current Movement: d");
                    prot.turnRight();
                }
                if(input == 'a') {
                    currentMovement.setText("Current Movement: a");
                    prot.turnLeft();
                }
                //set speed
                for (int i = 48; i < 58; i++) {
                    char s = (char) i;
                    System.out.println("set Speed: " + s);
                    if (input == s) {
                        currentMovement.setText("Speed: " + (i - 48));
                        //System.out.println("Speed: " + i);
                        prot.setM_speed(i - 48);
                    }
                }
                /*if(input == '1') {
                    currentMovement.setText("Current Movement: 1");
                    prot.setM_speed(1);
                }*/
            }
        });



        //finishing off the frame
        frame.setLocation(600, 400);

        frame.requestFocusInWindow();
        frame.setFocusable(true);

        frame.setVisible(true);
        frame.pack();
    }
}
