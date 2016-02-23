using Autodesk.Maya.OpenMaya;
using Autodesk.Maya.OpenMayaUI;

namespace KasMdl
{
	public class KDrawTraversal : MDrawTraversal
	{
		public enum TraversalFilter
		{
			kAll,
			kMeshes,
			kLights
		}
		public TraversalFilter Filter
		{
			get;
			set;
		}

		public KDrawTraversal()
		{
			Filter = TraversalFilter.kAll;
		}

		public override bool filterNode( MDagPath traversalItem )
		{
			bool prune = false;
			if( traversalItem.childCount == 0 )
			{
				switch( Filter )
				{
				case TraversalFilter.kMeshes:
					{
						if( !hasMeshes(traversalItem) )
						{
							prune = true;
						}
						break;
					}

				case TraversalFilter.kLights:
					{
						if( !hasLights(traversalItem) )
						{
							prune = true;
						}
						break;
					}

				case TraversalFilter.kAll:
					{
						if( !hasMeshes(traversalItem) && !hasLights(traversalItem) )
						{
							prune = true;
						}
						break;
					}
				}
			}
			return prune;
		}

		bool hasMeshes( MDagPath path )
		{
			return path.hasFn(MFn.Type.kMesh);
		}

		bool hasLights( MDagPath path )
		{
			return path.hasFn(MFn.Type.kDirectionalLight) ||
						 path.hasFn(MFn.Type.kPointLight) ||
						 path.hasFn(MFn.Type.kSpotLight);
		}
	}
}
