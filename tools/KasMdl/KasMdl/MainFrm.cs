using Autodesk.Maya.OpenMaya;
using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace KasMdl
{
	public partial class MainFrm : Form
	{
		public MainFrm()
		{
			InitializeComponent();
		}

		private void btn_browseOutFile_Click(object sender, EventArgs e)
		{
			SaveFileDialog sfd = new SaveFileDialog();
			sfd.Filter = "KasMdl Files (*.kmdl)|*.kmdl|All Files (*.*)|*.*";
			if( sfd.ShowDialog() == System.Windows.Forms.DialogResult.Cancel )
			{
				return;
			}

			txt_outFile.Text = sfd.FileName;
		}

		void btn_exportAllAscii_Click(object sender, EventArgs e)
		{
			throw new NotImplementedException("will get around to it eventually...maybe");
		}

		void btn_exportAllBinary_Click(object sender, EventArgs e)
		{
			MayaParser parser = new MayaParser();
			parser.parse();
			parser.writeBinary(txt_outFile.Text);
			MessageBox.Show("Exported to <" + txt_outFile.Text + ">!");
		}

		void btn_readBinary_Click(object sender, EventArgs e)
		{
			throw new NotImplementedException();

			tv_items.Nodes.Clear();
			



			//KasModel model = new KasModel();
			//model.readBinaryFile(txt_outFile.Text);

			//foreach( KasModel.Mesh currMesh in model.meshes )
			//{
			//	// Create new tab page and add a meshctrl to it.
			//	TabPage tp = new TabPage(currMesh.name);
			//	tc_readInfo.TabPages.Add(tp);
			//	MeshCtrl ctrl = new MeshCtrl();
			//	ctrl.Location = new System.Drawing.Point(0, 0);
			//	tp.Controls.Add(ctrl);

			//	// Set name of mesh.
			//	ctrl.lbl_meshName.Text = currMesh.name;

			//	// Add vertex data.
			//	foreach( KasModel.Vertex currVert in currMesh.vertices )
			//	{
			//		string pos = string.Format("Position({0}, {1}, {2})", currVert.position.x, currVert.position.y, currVert.position.z);
			//		string nrm = string.Format("Normal({0}, {1}, {2})", currVert.normal.x, currVert.normal.y, currVert.normal.z);
			//		string tex = string.Format("Texcoord({0}, {1})", currVert.texcoord.x, currVert.texcoord.y);
			//		string tan = string.Format("Tangent({0}, {1}, {2})", currVert.tangent.x, currVert.tangent.y, currVert.tangent.z);
			//		string bin = string.Format("Binormal({0}, {1}, {2})", currVert.bitangent.x, currVert.bitangent.y, currVert.bitangent.z);
			//		ctrl.lb_vertices.Items.Add(pos);
			//		ctrl.lb_vertices.Items.Add(nrm);
			//		ctrl.lb_vertices.Items.Add(tex);
			//		ctrl.lb_vertices.Items.Add(tan);
			//		ctrl.lb_vertices.Items.Add(bin);
			//	}

			//	// Add index data.
			//	foreach( int currIdx in currMesh.indices )
			//	{
			//		ctrl.lb_indices.Items.Add(currIdx.ToString());
			//	}
			//}

			//MessageBox.Show("Read binary file.");
		}
	}
}
