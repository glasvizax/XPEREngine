using Editor;

namespace WinFormsApp
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void InitializeComponent()
        {
            panel1 = new Panel();
            SuspendLayout();
            // 
            // panel1
            // 
            panel1.Location = new Point(165, 77);
            panel1.Name = "panel1";
            panel1.Size = new Size(360, 194);
            panel1.TabIndex = 0;
            // 
            // Form1
            // 
            ClientSize = new Size(704, 375);
            Controls.Add(panel1);
            MaximizeBox = false;
            MinimizeBox = false;
            Name = "Form1";
            Load += Form1_Load_1;
            ResumeLayout(false);
        }

        private Panel panel1;

        private void Form1_Load_1(object sender, EventArgs e)
        {
            var panelHandle = panel1.Handle;
            Task.Run(() =>
            {
                EngineBindings.runWinForms(panelHandle);
            });

        }
    }
}
