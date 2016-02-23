using System;
using System.IO;
using Autodesk.Maya.OpenMaya;
using System.Collections.Generic;

namespace KasMdl
{
	public class MayaParser
	{
		private MayaXform rootXform = new MayaXform();
		private List<MayaXform> allXforms = new List<MayaXform>();
		private Dictionary<string, MayaXform> pathXformMap = new Dictionary<string,MayaXform>();
		private List<MayaMesh> allMeshes = new List<MayaMesh>();
		private Dictionary<string, MayaMesh> pathMeshMap = new Dictionary<string,MayaMesh>();
		private List<string> xformsToIgnore = new List<string>();
		private List<MayaLight> allLights = new List<MayaLight>();
		private Dictionary<string, MayaLight> pathLightMap = new Dictionary<string,MayaLight>();

		public MayaParser()
		{
			xformsToIgnore.Add("|groundPlane_transform");
			xformsToIgnore.Add("|persp");
			xformsToIgnore.Add("|top");
			xformsToIgnore.Add("|front");
			xformsToIgnore.Add("|side");
		}

		public void parse()
		{
			buildXformTree();
			buildMeshNodes();
			computeMeshData();
			linkXformsMeshes();
			buildLightNodes();
		}

		public void writeBinary( string file )
		{
			BinaryWriter bw = new BinaryWriter(new FileStream(file, FileMode.Create));

			bw.Write(allMeshes.Count);
			foreach( MayaMesh mesh in allMeshes )
			{
				bw.Write(mesh.name);
				bw.Write(mesh.id);
				bw.Write(mesh.vertices.Count);
				foreach( Vertex vertex in mesh.vertices )
				{
					bw.Write(vertex.position.x); bw.Write(vertex.position.y); bw.Write(vertex.position.z);
					bw.Write(vertex.normal.x); bw.Write(vertex.normal.y); bw.Write(vertex.normal.z);
					bw.Write(vertex.texcoord.x);bw.Write(vertex.texcoord.y);
				}
				bw.Write(mesh.indices.Count);
				foreach( uint index in mesh.indices )
				{
					bw.Write(index);
				}
			}

			bw.Write(allXforms.Count);
			foreach( MayaXform xform in allXforms )
			{
				bw.Write(xform.name);
				bw.Write(xform.id);
				bw.Write(xform.parent == null ? -1 : xform.parent.id);

				// Write matrix.
				for( int x = 0; x < 4; ++x )
				{
					for( int y = 0; y < 4; ++y )
					{
						bw.Write(xform.matrix[x, y]);
					}
				}

				bw.Write(xform.isSourceXform);
				bw.Write((xform.mesh == null) ? -1 : xform.mesh.id);
				bw.Write(xform.children.Count);
				foreach( MayaXform child in xform.children )
				{
					bw.Write(child.id);
				}
			}

			bw.Write(allLights.Count);
			foreach( MayaLight light in allLights )
			{
				bw.Write(light.name);
				bw.Write(light.id);
				bw.Write(light.sourceXform == null ? -1 : light.sourceXform.id);
				bw.Write((int)light.type); //0=invalid; 1=ambient; 2=directional; 3=point; 4=spot
				bw.Write(light.color.r); bw.Write(light.color.g); bw.Write(light.color.b); bw.Write(light.color.a);
				bw.Write(light.intensity);
				if( light.type == MayaLight.Type.kPoint )
				{
					bw.Write(light.range);
				}
				else if( light.type == MayaLight.Type.kSpot )
				{
					bw.Write(light.range);
					bw.Write(light.coneInnerAngle);
					bw.Write(light.coneOuterAngle);
				}
			}

			bw.Close();
		}

		#region read binary (unfinished)
		public void readBinaryFile( string file )
		{
			// TODO: Reset data.

			BinaryReader br = new BinaryReader(new FileStream(file, FileMode.Open));
			
			// Mesh count.
			int meshCount = br.ReadInt32();
			// Read all meshes.
			for( int currMesh = 0; currMesh < meshCount; ++currMesh )
			{
				MayaMesh mesh = new MayaMesh();
				// Mesh name.
				mesh.name = br.ReadString();
				//Mesh id.
				mesh.id = br.ReadInt32();
				// Vertex count.
				int vertexCount = br.ReadInt32();
				// Read all vertices.
				for( int currVert = 0; currVert < vertexCount; ++currVert )
				{
					Vertex vert = new Vertex();
					// Read position.
					vert.position.x = br.ReadSingle();
					vert.position.y = br.ReadSingle();
					vert.position.z = br.ReadSingle();
					// Read normal.
					vert.normal.x = br.ReadSingle();
					vert.normal.y = br.ReadSingle();
					vert.normal.z = br.ReadSingle();
					// Read texcoord.
					vert.texcoord.x = br.ReadSingle();
					vert.texcoord.y = br.ReadSingle();

					mesh.vertices.Add(vert);
				}
				// Read index count.
				int indexCount = br.ReadInt32();
				// Read all indices.
				for( int currIdx = 0; currIdx < indexCount; ++currIdx )
				{
					mesh.indices.Add(br.ReadUInt32());
				}

				allMeshes.Add(mesh);
			}

			// Read xform count.
			int xformCount = br.ReadInt32();
			for( int currXform = 0; currXform < xformCount; ++currXform )
			{
				MayaXform xform = new MayaXform();
				// Read name.
				xform.name = br.ReadString();
				// Read id.
				xform.id = br.ReadInt32();
				// Read parent id.
				int parentId = br.ReadInt32();
				if( parentId != -1 )
				{
					xform.parent = allXforms[parentId];
				}
				// Read matrix.
				for( int x = 0; x < 4; ++x )
				{
					for( int y = 0; y < 4; ++y )
					{
						xform.matrix[x,y] = br.ReadSingle();
					}
				}
				// Read source xform bool.
				xform.isSourceXform = br.ReadBoolean();
				// Read mesh id.
				int meshId = br.ReadInt32();
				if( meshId != -1 )
				{
					xform.mesh = allMeshes[meshId];
				}
				// Read children count.
				int childCount = br.ReadInt32();
				// Read all children ids.
				for( int currChild = 0; currChild < childCount; ++currChild )
				{
					int childId = br.ReadInt32();
					xform.children.Add(allXforms[childId]);
				}

				allXforms.Add(xform);
			}

			br.Close();
		}
		#endregion

		private void getXformData( MayaXform xform )
		{
			// Get the maya transform node to get data from.
			MFnTransform mayaXform = new MFnTransform(xform.mayaObjectPath);

			// Get the matrix.
			MMatrix mayaMatrix = mayaXform.transformationMatrix;
			mayaMatrix.get(xform.matrix);
		}

		private void processXform( MayaXform xform )
		{
			// Add the current Xform to the list of all xforms.
			allXforms.Add(xform);

			// Set the Xform's id.
			xform.id = allXforms.Count - 1;

			// Add the current Xform to the map based on its path.
			pathXformMap[xform.name] = xform;

			// Build data for this xform.
			if( !xform.isRoot )
			{
				getXformData(xform);
			}

			MFnDagNode dagNode = new MFnDagNode(xform.mayaObjectPath);
			for( uint childIdx = 0; childIdx < dagNode.childCount; ++childIdx )
			{
				// If the child isn't a transform node, or it is a kManipulator3D (maya api bug), skip it.
				MObject childObj = dagNode.child(childIdx);
				if( !childObj.hasFn(MFn.Type.kTransform) || childObj.hasFn(MFn.Type.kManipulator3D) )
				{
					continue;
				}

				// Create a node for the child transform, parent it to this xform, and process the child's children.
				MFnDagNode childNode = new MFnDagNode(childObj);
				// Strip out nodes we don't care about.
				if( xformsToIgnore.Contains(childNode.fullPathName) )
				{
					continue;
				}
				MayaXform childXform = new MayaXform();
				childXform.name = childNode.fullPathName;
				childNode.getPath(childXform.mayaObjectPath);
				childXform.parent = xform;
				xform.children.Add(childXform);
				processXform(childXform);
			}
		}

		private void buildXformTree()
		{
			// Get the very root of the Maya DAG.
			MItDag rootIt = new MItDag();
			rootXform.isRoot = true;
			rootXform.name = "root";
			MFnDagNode rootDagNode = new MFnDagNode(rootIt.root());
			rootDagNode.getPath(rootXform.mayaObjectPath);

			// Process all children transforms of the root.
			processXform(rootXform);
		}

		private void buildMeshNodes()
		{
			MItDependencyNodes it = new MItDependencyNodes(MFn.Type.kMesh);
			while( !it.isDone )
			{
				// Get the dag node of the mesh.
				MFnDagNode dagNode = new MFnDagNode(it.thisNode);
				// Create a new mesh.
				MayaMesh meshNode = new MayaMesh();
				// Add the mesh to the all meshes list.
				allMeshes.Add(meshNode);
				// Configure the mesh node
				meshNode.name = dagNode.fullPathName;
				dagNode.getPath(meshNode.mayaObjectPath);
				meshNode.id = allMeshes.Count - 1;
				// The first parent of a mesh is the source transform, even with instances.
				meshNode.sourceXForm = pathXformMap[(new MFnDagNode(dagNode.parent(0))).fullPathName];
				// Set the source Xform to know it's a source for an instance (its mesh).
				meshNode.sourceXForm.isSourceXform = true;
				// If the mesh has more than one parent, it has been instanced.  We need to know this for computing local positions.
				meshNode.isInstanced = dagNode.isInstanced();
				// Add the map to the dictionary to search by name.
				pathMeshMap[meshNode.name] = meshNode;
				it.next();
			}
		}

		private void computeMeshData()
		{
			foreach( MayaMesh mesh in allMeshes )
			{
				// Get the Maya mesh
				MFnMesh mayaMesh = new MFnMesh(mesh.mayaObjectPath);

				// Does the maya mesh have UVs?
				MStringArray uvSetNames = new MStringArray();
				mayaMesh.getUVSetNames(uvSetNames);
				bool hasUvs = (uvSetNames.length > 0) && (mayaMesh.numUVs(uvSetNames[0]) > 0);

				// Iterate through all of the vertices and build the data.
				MItMeshFaceVertex it = new MItMeshFaceVertex(mesh.mayaObjectPath);
				while( !it.isDone )
				{
					// Create a new vertex and populate its data.
					Vertex vert = new Vertex();

					// Get the local position relative to the world origin.
					MPoint mayaPos = it.position(MSpace.Space.kObject);
					vert.position = new Vector3((float)mayaPos.x, (float)mayaPos.y, (float)mayaPos.z);
					//vert.position = new Vector3((float)mayaPos.x - mesh.sourceXForm.mayaWorldPosition.x,
																			//(float)mayaPos.y - mesh.sourceXForm.mayaWorldPosition.y,
																			//(float)mayaPos.z - mesh.sourceXForm.mayaWorldPosition.z);

					// Get the normal.
					MVector mayaNrm = new MVector();
					it.getNormal(mayaNrm, MSpace.Space.kObject);
					vert.normal = new Vector3((float)mayaNrm.x, (float)mayaNrm.y, (float)mayaNrm.z);

					// Texcoords.
					if( hasUvs && it.hasUVsProperty )
					{
						float[] mayaUvs = new float[2];
						it.getUV(mayaUvs, uvSetNames[0]);
						vert.texcoord = new Vector2(mayaUvs[0], mayaUvs[1]);
					}

					// Append the vertex.
					mesh.vertices.Add(vert);
					it.next();
				}

				// Get all index data.
				MIntArray mia1 = new MIntArray();
				MIntArray mia2 = new MIntArray();
				mayaMesh.getTriangleOffsets(mia1, mia2);
				foreach( int idx in mia2 )
				{
					mesh.indices.Add((uint)idx);
				}
			}
		}

		private void linkXformsMeshes()
		{
			foreach( MayaXform xform in allXforms )
			{
				// Convert to a DAG node to traverse children.
				MFnDagNode dagNode = new MFnDagNode(xform.mayaObjectPath);
				for( uint childIdx = 0; childIdx < dagNode.childCount; ++childIdx )
				{
					MObject childObj = dagNode.child(childIdx);
					// If the child has a mesh, link it.
					if( childObj.hasFn(MFn.Type.kMesh) )
					{
						xform.mesh = pathMeshMap[(new MFnMesh(childObj)).fullPathName];
						break;
					}
				}
			}
		}

		private bool shouldPruneLight( MObject obj )
		{
			return !(obj.hasFn(MFn.Type.kAmbientLight) || obj.hasFn(MFn.Type.kDirectionalLight) ||
							 obj.hasFn(MFn.Type.kPointLight) || obj.hasFn(MFn.Type.kSpotLight));
		}

		private void buildLightNodes()
		{
			MItDependencyNodes it = new MItDependencyNodes(MFn.Type.kLight);
			while( !it.isDone )
			{
				MObject mayaObj = it.thisNode;
				if( shouldPruneLight(mayaObj) )
				{
					it.next();
					continue;
				}

				// Create a new light and add it to the all lights list.
				MayaLight light = new MayaLight();
				allLights.Add(light);
				light.id = allLights.Count - 1;
				// Get the dag node of the light for its name and parent.
				MFnDagNode dagNode = new MFnDagNode(mayaObj);
				dagNode.getPath(light.mayaObjectPath);
				light.name = dagNode.fullPathName;
				// First parent is the source transform node.
				light.sourceXform = pathXformMap[(new MFnDagNode(dagNode.parent(0))).fullPathName];
				// Add the light to the dictionary to search by name.
				pathLightMap[light.name] = light;

				//
				// NOTE: Parent transforms of light sources in Maya can NOT be frozen,
				//       so accessing them is guaranteed to not be frozen.
				//

				if( mayaObj.hasFn(MFn.Type.kAmbientLight) )
				{
					MFnAmbientLight mayaLight = new MFnAmbientLight(mayaObj);
					light.type = MayaLight.Type.kAmbient;
					light.color = new Color(mayaLight.color.r, mayaLight.color.g, mayaLight.color.b, mayaLight.color.a);
					light.intensity = mayaLight.intensity;
				}
				else if( mayaObj.hasFn(MFn.Type.kDirectionalLight) )
				{
					MFnDirectionalLight mayaLight = new MFnDirectionalLight(mayaObj);
					light.type = MayaLight.Type.kDirectional;
					light.color = new Color(mayaLight.color.r, mayaLight.color.g, mayaLight.color.b, mayaLight.color.a);
					light.intensity = mayaLight.intensity;
				}
				else if( mayaObj.hasFn(MFn.Type.kPointLight) )
				{
					MFnPointLight mayaLight = new MFnPointLight(mayaObj);
					light.type = MayaLight.Type.kPoint;
					light.color = new Color(mayaLight.color.r, mayaLight.color.g, mayaLight.color.b, mayaLight.color.a);
					light.intensity = mayaLight.intensity;
					light.range = (float)mayaLight.centerOfIllumination;
				}
				else if( mayaObj.hasFn(MFn.Type.kSpotLight) )
				{
					MFnSpotLight mayaLight = new MFnSpotLight(mayaObj);
					light.type = MayaLight.Type.kSpot;
					light.color = new Color(mayaLight.color.r, mayaLight.color.g, mayaLight.color.b, mayaLight.color.a);
					light.intensity = mayaLight.intensity;
					light.range = (float)mayaLight.centerOfIllumination;
					float mayaConeAngle = (float)(mayaLight.coneAngle * 0.5);
					light.coneInnerAngle = 57.29578f * mayaConeAngle;
					light.coneOuterAngle = 57.29578f * (mayaConeAngle + Math.Abs((float)mayaLight.penumbraAngle));
				}

				it.next();
			}
		}
	}
}
