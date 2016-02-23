namespace KasMdl
{
	partial class MainFrm
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.btn_exportAllAscii = new System.Windows.Forms.Button();
			this.txt_outFile = new System.Windows.Forms.TextBox();
			this.btn_browseOutFile = new System.Windows.Forms.Button();
			this.btn_exportAllBinary = new System.Windows.Forms.Button();
			this.btn_readBinary = new System.Windows.Forms.Button();
			this.gb_readData = new System.Windows.Forms.GroupBox();
			this.tv_items = new System.Windows.Forms.TreeView();
			this.gb_readData.SuspendLayout();
			this.SuspendLayout();
			// 
			// btn_exportAllAscii
			// 
			this.btn_exportAllAscii.Location = new System.Drawing.Point(12, 42);
			this.btn_exportAllAscii.Name = "btn_exportAllAscii";
			this.btn_exportAllAscii.Size = new System.Drawing.Size(223, 40);
			this.btn_exportAllAscii.TabIndex = 0;
			this.btn_exportAllAscii.Text = "Export As ASCII";
			this.btn_exportAllAscii.UseVisualStyleBackColor = true;
			this.btn_exportAllAscii.Click += new System.EventHandler(this.btn_exportAllAscii_Click);
			// 
			// txt_outFile
			// 
			this.txt_outFile.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.txt_outFile.Location = new System.Drawing.Point(12, 12);
			this.txt_outFile.Name = "txt_outFile";
			this.txt_outFile.Size = new System.Drawing.Size(393, 24);
			this.txt_outFile.TabIndex = 1;
			// 
			// btn_browseOutFile
			// 
			this.btn_browseOutFile.Location = new System.Drawing.Point(411, 12);
			this.btn_browseOutFile.Name = "btn_browseOutFile";
			this.btn_browseOutFile.Size = new System.Drawing.Size(41, 24);
			this.btn_browseOutFile.TabIndex = 2;
			this.btn_browseOutFile.Text = "...";
			this.btn_browseOutFile.UseVisualStyleBackColor = true;
			this.btn_browseOutFile.Click += new System.EventHandler(this.btn_browseOutFile_Click);
			// 
			// btn_exportAllBinary
			// 
			this.btn_exportAllBinary.Location = new System.Drawing.Point(241, 42);
			this.btn_exportAllBinary.Name = "btn_exportAllBinary";
			this.btn_exportAllBinary.Size = new System.Drawing.Size(211, 40);
			this.btn_exportAllBinary.TabIndex = 0;
			this.btn_exportAllBinary.Text = "Export As Binary";
			this.btn_exportAllBinary.UseVisualStyleBackColor = true;
			this.btn_exportAllBinary.Click += new System.EventHandler(this.btn_exportAllBinary_Click);
			// 
			// btn_readBinary
			// 
			this.btn_readBinary.Location = new System.Drawing.Point(12, 88);
			this.btn_readBinary.Name = "btn_readBinary";
			this.btn_readBinary.Size = new System.Drawing.Size(440, 41);
			this.btn_readBinary.TabIndex = 3;
			this.btn_readBinary.Text = "Read Binary File";
			this.btn_readBinary.UseVisualStyleBackColor = true;
			this.btn_readBinary.Click += new System.EventHandler(this.btn_readBinary_Click);
			// 
			// gb_readData
			// 
			this.gb_readData.Controls.Add(this.tv_items);
			this.gb_readData.Location = new System.Drawing.Point(12, 135);
			this.gb_readData.Name = "gb_readData";
			this.gb_readData.Size = new System.Drawing.Size(440, 393);
			this.gb_readData.TabIndex = 4;
			this.gb_readData.TabStop = false;
			this.gb_readData.Text = "Read Data";
			// 
			// tv_items
			// 
			this.tv_items.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tv_items.Location = new System.Drawing.Point(3, 16);
			this.tv_items.Name = "tv_items";
			this.tv_items.Size = new System.Drawing.Size(434, 374);
			this.tv_items.TabIndex = 0;
			// 
			// MainFrm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(464, 536);
			this.Controls.Add(this.gb_readData);
			this.Controls.Add(this.btn_readBinary);
			this.Controls.Add(this.btn_browseOutFile);
			this.Controls.Add(this.txt_outFile);
			this.Controls.Add(this.btn_exportAllBinary);
			this.Controls.Add(this.btn_exportAllAscii);
			this.Name = "MainFrm";
			this.Text = "MainFrm";
			this.gb_readData.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button btn_exportAllAscii;
		private System.Windows.Forms.TextBox txt_outFile;
		private System.Windows.Forms.Button btn_browseOutFile;
		private System.Windows.Forms.Button btn_exportAllBinary;
		private System.Windows.Forms.Button btn_readBinary;
		private System.Windows.Forms.GroupBox gb_readData;
		private System.Windows.Forms.TreeView tv_items;

	}
}