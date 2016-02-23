namespace KasMdl
{
	partial class MeshCtrl
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

		#region Component Designer generated code

		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.lbl_nameLbl = new System.Windows.Forms.Label();
			this.lbl_meshName = new System.Windows.Forms.Label();
			this.lb_vertices = new System.Windows.Forms.ListBox();
			this.gb_vertices = new System.Windows.Forms.GroupBox();
			this.gb_indices = new System.Windows.Forms.GroupBox();
			this.lb_indices = new System.Windows.Forms.ListBox();
			this.gb_vertices.SuspendLayout();
			this.gb_indices.SuspendLayout();
			this.SuspendLayout();
			// 
			// lbl_nameLbl
			// 
			this.lbl_nameLbl.AutoSize = true;
			this.lbl_nameLbl.Location = new System.Drawing.Point(3, 11);
			this.lbl_nameLbl.Name = "lbl_nameLbl";
			this.lbl_nameLbl.Size = new System.Drawing.Size(65, 13);
			this.lbl_nameLbl.TabIndex = 0;
			this.lbl_nameLbl.Text = "Mesh name:";
			// 
			// lbl_meshName
			// 
			this.lbl_meshName.AutoSize = true;
			this.lbl_meshName.Location = new System.Drawing.Point(74, 11);
			this.lbl_meshName.Name = "lbl_meshName";
			this.lbl_meshName.Size = new System.Drawing.Size(16, 13);
			this.lbl_meshName.TabIndex = 1;
			this.lbl_meshName.Text = "...";
			// 
			// lb_vertices
			// 
			this.lb_vertices.Dock = System.Windows.Forms.DockStyle.Fill;
			this.lb_vertices.FormattingEnabled = true;
			this.lb_vertices.Location = new System.Drawing.Point(3, 16);
			this.lb_vertices.Name = "lb_vertices";
			this.lb_vertices.Size = new System.Drawing.Size(399, 127);
			this.lb_vertices.TabIndex = 2;
			// 
			// gb_vertices
			// 
			this.gb_vertices.Controls.Add(this.lb_vertices);
			this.gb_vertices.Location = new System.Drawing.Point(6, 27);
			this.gb_vertices.Name = "gb_vertices";
			this.gb_vertices.Size = new System.Drawing.Size(405, 146);
			this.gb_vertices.TabIndex = 3;
			this.gb_vertices.TabStop = false;
			this.gb_vertices.Text = "Vertices";
			// 
			// gb_indices
			// 
			this.gb_indices.Controls.Add(this.lb_indices);
			this.gb_indices.Location = new System.Drawing.Point(6, 179);
			this.gb_indices.Name = "gb_indices";
			this.gb_indices.Size = new System.Drawing.Size(405, 146);
			this.gb_indices.TabIndex = 3;
			this.gb_indices.TabStop = false;
			this.gb_indices.Text = "Indices";
			// 
			// lb_indices
			// 
			this.lb_indices.Dock = System.Windows.Forms.DockStyle.Fill;
			this.lb_indices.FormattingEnabled = true;
			this.lb_indices.Location = new System.Drawing.Point(3, 16);
			this.lb_indices.Name = "lb_indices";
			this.lb_indices.Size = new System.Drawing.Size(399, 127);
			this.lb_indices.TabIndex = 2;
			// 
			// MeshCtrl
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.gb_indices);
			this.Controls.Add(this.gb_vertices);
			this.Controls.Add(this.lbl_meshName);
			this.Controls.Add(this.lbl_nameLbl);
			this.Name = "MeshCtrl";
			this.Size = new System.Drawing.Size(414, 334);
			this.gb_vertices.ResumeLayout(false);
			this.gb_indices.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label lbl_nameLbl;
		public System.Windows.Forms.Label lbl_meshName;
		public System.Windows.Forms.ListBox lb_vertices;
		private System.Windows.Forms.GroupBox gb_vertices;
		private System.Windows.Forms.GroupBox gb_indices;
		public System.Windows.Forms.ListBox lb_indices;
	}
}
