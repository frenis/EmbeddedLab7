using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp_Example
{
    public partial class Form1 : Form
    {
        int startflag = 0;
        int flag_sensor;
        string RxString;
        string temp;

        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            
            serialPort1.PortName = "COM5";
            serialPort1.BaudRate = 9600;
            serialPort1.Open();
            if (serialPort1.IsOpen)
            {
                //startSerial.Enabled = false;
                //serialStop.Enabled = true;
                textBox1.ReadOnly = false;
            }

        }

        private void Form1_Load(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)
                serialPort1.Close();
            serialPort1.PortName = "COM5";
            serialPort1.BaudRate = 9600;
        }

        private void label2_Click(object sender, EventArgs e) //Current Data
        {
            textBox1.AppendText(RxString);
        }

        private void Serial_stop_Click(object sender, EventArgs e)
        {
            serialPort1.Close();
            //  startSerial.Enabled = true;
            //  serialStop.Enabled = false;
            textBox1.ReadOnly = true;
        }

        private void Read_in_TS_Click(object sender, EventArgs e)
        {
            WebClient client = new WebClient();
            //ThingSpeakData.Text = 
            client.DownloadString("http://api.thingspeak.com/channels/[channel id]/feed.json");
            label1.Text = client.DownloadString("http://api.thingspeak.com/channels/[channel id]/field/field1/last.text");
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (!string.Equals(textBox1.Text, ""))
            {
                if (serialPort1.IsOpen) serialPort1.Close();
                try
                {
                     if (RxString[0] == 'A')
                      {
                          flag_sensor = 11;
                      }
                      const string WRITEKEY = "[Thingspeak Write key]";
                    string strUpdateBase = "http://api.thingspeak.com/update";
                    string strUpdateURI = strUpdateBase + "?api_key=" + WRITEKEY;
                    string strField1 = textBox1.Text;
                    // string strField2 = "42";
                    HttpWebRequest ThingsSpeakReq;
                    HttpWebResponse ThingsSpeakResp;
                    //strUpdateURI += "&field1=" + strField1;
                    if (flag_sensor == 12)
                    {
                        strUpdateURI += "&field1=" + strField1;
                    }
                    else if (flag_sensor == 13)
                    {
                        strUpdateURI += "&field2=" + strField1;
                    }
                    /*else if (flag_sensor == 13)
                    {
                        strUpdateURI += "&field3=" + strField1;
                    }
                    else if (flag_sensor == 14)
                    {
                        strUpdateURI += "&field4=" + strField1;
                    }
                    else
                    {
                    }*/
                    flag_sensor++;
                  
                    ThingsSpeakReq = (HttpWebRequest)WebRequest.Create(strUpdateURI);
                    ThingsSpeakResp = (HttpWebResponse)ThingsSpeakReq.GetResponse();
                    ThingsSpeakResp.Close();
                    if (!(string.Equals(ThingsSpeakResp.StatusDescription, "OK")))
                    {
                        Exception exData = new Exception(ThingsSpeakResp.StatusDescription);
                        throw exData;
                    }
                }
                catch (Exception ex)
                {
                }
                textBox1.Text = "";
                serialPort1.Open();
            }
        }

        private void SerialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            RxString = serialPort1.ReadExisting();
            this.Invoke(new EventHandler(label2_Click));
        }
    }
}
