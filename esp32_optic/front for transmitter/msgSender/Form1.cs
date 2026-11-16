using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace msgSender
{
    public partial class Form1 : Form
    {

        string ipAddress = null;
        int port = 8008;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                if (ipAddress == null)
                {
                    MessageBox.Show("Please enter a valid IP address.");
                    return;
                }
          
                Console.WriteLine($"Connecting to ESP at {ipAddress}:{port}...");

                using (TcpClient client = new TcpClient(ipAddress, port))
                {
                    Console.WriteLine("Connected! Sending message...");
                    NetworkStream stream = client.GetStream();

                    stream.ReadTimeout = 3000;

                    byte[] dataToSend = Encoding.UTF8.GetBytes(richTextBox1.Text);
                    stream.Write(dataToSend, 0, dataToSend.Length);
                    Console.WriteLine($"Sent: {richTextBox1.Text.Trim()}");

                    byte[] buffer = new byte[1024];
                    int bytesRead = stream.Read(buffer, 0, buffer.Length);

                    if (bytesRead > 0)
                    {
                        string response = Encoding.UTF8.GetString(buffer, 0, bytesRead);
                        MessageBox.Show(response);
                    }
                }

                Console.WriteLine("Connection closed.");
            }
            catch (Exception ex)
            {
                MessageBox.Show($"error: {ex.Message}");
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            ipAddress = textBoxIp.Text;
        }
    }
}
