/* BZWorkbench
 * Copyright (c) 1993 - 2008 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "objects/mesh.h"

struct TriIndices {
  int indices[3];
};

struct MeshVertex {
	osg::Vec3 vertex;
	osg::Vec3 normal;
	osg::Vec2 texcoord;
};

// default constructor
mesh::mesh(void) :
	bz2object("mesh", "<name><position><size><rotation><vertex><normal><texcoord><inside><outside><shift><scale><shear><spin><phydrv><smoothbounce><noclusters><face><drawinfo><drivethrough><shootthrough><passable>") {

	decorative = false;
	faces = vector<MeshFace*>();
	drawInfo = NULL;
	currentDrawInfo = NULL;
	currentFace = NULL;
	currentMaterial = NULL;
	noclusters = false;
	phydrv = NULL;
	smoothbounce = false;
	drivethrough = false;
	shootthrough = false;
}


// getter
string mesh::get(void) { return toString(); }

// event handler
int mesh::update( UpdateMessage& message ) {
	// superclass update (i.e. handle transformation changes)
	int result = bz2object::update( message );
	// NOW handle the messages
	switch( message.type ) {		
		case UpdateMessage::SET_POSITION: 
			setPos( *(message.getAsPosition()) );
			break;
		case UpdateMessage::SET_POSITION_FACTOR: 	// handle a translation
			setPos( this->getPos() + *(message.getAsPositionFactor()) );
			break;
		case UpdateMessage::SET_ROTATION:		// handle a new rotation
			setRotation( *(message.getAsRotation()) );
			break;
		case UpdateMessage::SET_ROTATION_FACTOR:	// handle an angular translation
			setRotation( this->getRotation() + *(message.getAsRotationFactor()) );
			break;
		case UpdateMessage::SET_SCALE:		// handle a new scale
			setSize( *(message.getAsScale()) );
			updateGeometry();			
			break;
		case UpdateMessage::SET_SCALE_FACTOR:	// handle a scaling factor
			setSize( getSize() +  *(message.getAsScale()) );
			updateGeometry();
			break;
		default:	// unknown event; don't handle
			return result;
	}
	return 1;	
}

// bzw methods
bool mesh::parse( std::string& line ) {
	string key = BZWParser::key( line.c_str() );
	string value = BZWParser::value( key.c_str(), line.c_str() );
	
	if ( currentDrawInfo ) {
		if ( !currentDrawInfo->parse( line ) ) {
			drawInfo = currentDrawInfo;
			currentDrawInfo = NULL;
		}
	}
	else if ( key == "mesh" )
		return true;
	else if ( key == "lod" ) {
		lodOptions.push_back( value );
	}
	else if ( currentFace ) {
		if ( !currentFace->parse( line ) ) {
			faces.push_back(currentFace);
			currentFace = NULL;
		}
	}
	else if ( key == "face" ) {
		if (currentFace != NULL) {
			printf("mesh::update(): Warning! Discarding incomplete mesh face.\n");
			delete currentFace;
		}
		currentFace = new MeshFace( currentMaterial, phydrv, noclusters, smoothbounce, drivethrough, shootthrough );
	}
	else if ( key == "inside" ) {
		insidePoints.push_back( Point3D( value.c_str() ) );
	}
	else if ( key == "outside" ) {
		outsidePoints.push_back( Point3D( value.c_str() ) );
	}
	else if ( key == "vertex" ) {
		vertices.push_back( Point3D( value.c_str() ) );
	}
	else if ( key == "normal" ) {
		normals.push_back( Point3D( value.c_str() ) );
	}
	else if ( key == "texcoord" ) {
		texCoords.push_back( Point2D( value.c_str() ) );
	}
	else if ( key == "phydrv" ) {
		physics* phys = (physics*)Model::command( MODEL_GET, "phydrv", value.c_str() );
		if (phys != NULL)
			phydrv = phys;
		else
			throw BZWReadError( this, string( "Couldn't find physics driver, " ) + value );
	}
	else if ( key == "noclusters" ) {
		noclusters = true;
	}
	else if ( key == "decorative" ) {
		decorative = true;
	}
	else if ( key == "drawinfo" ) {
		if (drawInfo != NULL) {
			throw BZWReadError( this, "Multiple drawinfo sections." );
		}
		else {
			currentDrawInfo = new DrawInfo();
		}
	}
	else if ( key == "matref" ) {
		material* mat = dynamic_cast< material* >( Model::command( MODEL_GET, "material", value ) );

		if ( mat )
			currentMaterial = mat;
		else
			throw BZWReadError( this, string( "Couldn't find material, " ) + value );
	}
	else if ( key == "end" ) // need to check if we're at the end of the section
		return false;
	else {
		return bz2object::parse( line );
	}

	return true;
}

void mesh::finalize() {
	
	bz2object::finalize();
	updateGeometry();
}

// to string
string mesh::toString(void) {
	// string-ify the vertices, normals, texcoords, inside points, outside points, passibility and faces
	string vertexString(""), normalString(""), texcoordString(""), insideString(""), outsideString(""),passabilityString(""), faceString("");

	if(vertices.size() > 0) {
		for(vector<Point3D>::iterator i = vertices.begin(); i != vertices.end(); i++) {
			vertexString += "  vertex " + i->toString();
		}
	}

	if(normals.size() > 0) {
		for(vector<Point3D>::iterator i = normals.begin(); i != normals.end(); i++) {
			normalString += "  normal " + i->toString();
		}
	}

	if(texCoords.size() > 0) {
		for(vector<Point2D>::iterator i = texCoords.begin(); i != texCoords.end(); i++) {
			texcoordString += "  texcoord " + i->toString() + "\n";
		}
	}

	// special instance:
	// make sure to keep the order of faces and materials constant
	if(faces.size() > 0) {
		for(vector<MeshFace*>::iterator i = faces.begin(); i != faces.end(); i++) {
			faceString += (*i)->toString();
		}
	}

	if(insidePoints.size() > 0) {
		for(vector<Point3D>::iterator i = insidePoints.begin(); i != insidePoints.end(); i++) {
			insideString += "  inside " + i->toString();
		}
	}

	if(outsidePoints.size() > 0) {
		for(vector<Point3D>::iterator i = outsidePoints.begin(); i != outsidePoints.end(); i++) {
			outsideString += "  outside " + i->toString();
		}
	}
	
	if(drivethrough && shootthrough) {
		passabilityString = "  passable\n";
	}else{
		if(drivethrough)
			passabilityString = "  drivethrough\n";
		if(shootthrough)
			passabilityString = "  shootthrough\n";
	}

	return string("mesh\n") +
				  BZWLines( this ) +
				  insideString +
				  outsideString +
				  vertexString +
				  normalString +
				  texcoordString +
				  passabilityString +
				  faceString +
				  (drawInfo != NULL ? "  " + drawInfo->toString() : "") + "\n" +
				  "end\n";
}

// render
int mesh::render(void) {
	return 0;
}

void mesh::updateGeometry() {
	osg::Group* group = new osg::Group();

	map< material*, osg::Geometry* > geomMap;

	for ( vector< MeshFace* >::iterator i = faces.begin(); i != faces.end(); i++ ) {
		MeshFace* face = *i;
		material* mat = face->getMaterial();
		
		osg::Geometry* geom;
		if ( geomMap.count( mat ) > 0 ) {
			geom = geomMap[ mat ];
		}
		else {
			geom = new osg::Geometry();
			geom->setVertexArray( new osg::Vec3Array() );
			geomMap[ mat ] = geom;
		}

		// get the vertices for the current face
		vector<int> vertIndices = face->getVertices();
		vector<int> normalIndices = face->getNormals();
		vector<int> texcoordIndices = face->getTexcoords();
		bool hasNormals = false, hasTexcoords = false;

		// get the arrays
		osg::Vec3Array* verts = (osg::Vec3Array*)geom->getVertexArray();
		osg::Vec3Array* norms = (osg::Vec3Array*)geom->getNormalArray();
		osg::Vec2Array* tcoords = NULL;
		if ( geom->getNumTexCoordArrays() > 0 )
			tcoords = (osg::Vec2Array*)geom->getTexCoordArray( 0 );

		// check if there are normals and texcoords and if there are the right amount
		if ( normalIndices.size() > 0 ) {
			if ( normalIndices.size() == vertIndices.size() ) {
				hasNormals = true;

				// if there isn't a normals array make one
				if ( norms == NULL ) {
					norms = new osg::Vec3Array();
					geom->setNormalArray( norms );
				}
			}
			else
				throw BZWReadError( this, "Number of normals doesn't match number of vertices." );
		}

		if ( texcoordIndices.size() > 0 ) {
			if ( texcoordIndices.size() == vertIndices.size() ) {
				hasTexcoords = true;
				
				// if there isn't a texcoords array make one
				if ( tcoords == NULL ) {
					tcoords = new osg::Vec2Array();
					geom->setTexCoordArray( 0, tcoords );
				}
			}
			else
				throw BZWReadError( this, "Number of textcoords doesn't match number of vertices." );
		}

		vector<MeshVertex> faceVertices;
		vector<int>::iterator v = vertIndices.begin();
		vector<int>::iterator n = normalIndices.begin();
		vector<int>::iterator t = texcoordIndices.begin();
		for ( ; v != vertIndices.end(); v++ ) {
			MeshVertex vtx;
			vtx.vertex = vertices[ *v ];
			if ( hasNormals ) {
				vtx.normal = normals[ *n ];
				n++;
			}
			if ( hasTexcoords ) {
				vtx.texcoord = texCoords[ *t ];
				t++;
			}
			faceVertices.push_back( vtx );
		}

		// triangulate the face
		vector<TriIndices> indices;
		triangulateFace( faceVertices, indices );

		// add the vertices to the face
		osg::DrawElementsUInt* drawElem = new osg::DrawElementsUInt( osg::DrawElements::TRIANGLES, 0 );
		for ( vector<TriIndices>::iterator j = indices.begin(); j != indices.end(); j++ ) {
			verts->push_back( faceVertices[ (*j).indices[0] ].vertex );
			if ( hasNormals ) norms->push_back( faceVertices[ (*j).indices[0] ].normal );
			if ( hasTexcoords ) tcoords->push_back( faceVertices[ (*j).indices[0] ].texcoord );
			drawElem->push_back( verts->size() - 1 );
			verts->push_back( faceVertices[ (*j).indices[1] ].vertex );
			if ( hasNormals ) norms->push_back( faceVertices[ (*j).indices[1] ].normal );
			if ( hasTexcoords ) tcoords->push_back( faceVertices[ (*j).indices[1] ].texcoord );
			drawElem->push_back( verts->size() - 1 );
			verts->push_back( faceVertices[ (*j).indices[2] ].vertex );
			if ( hasNormals ) norms->push_back( faceVertices[ (*j).indices[2] ].normal );
			if ( hasTexcoords ) tcoords->push_back( faceVertices[ (*j).indices[2] ].texcoord );
			drawElem->push_back( verts->size() - 1 );
		}
   
		geom->addPrimitiveSet( drawElem );
	}

	for ( map< material*, osg::Geometry* >::iterator i = geomMap.begin(); i != geomMap.end(); i++ ) {
		osg::Geode* geode = new osg::Geode();
		material* mat = i->first;
		if(mat == NULL){
			// set default material of face to be invisible if there isn't one defined yet
			mat = new material();
			mat->setDiffuse( osg::Vec4( 1.0, 1.0, 1.0, 0.0) );
		}
		geode->addDrawable( i->second );
		geode->setStateSet( mat );
		group->addChild( geode );
	}

	setThisNode( group );
}


/*
 * Trangulation stuff
 */

// triangulation parameters
static osg::Vec3 Normal; // FIXME, uNormal, vNormal;
//static fvec2* MVertsSpace = NULL;
static const vector<MeshVertex>* Verts;
static int Count = 0;
static int* WorkSet = NULL;


static inline bool makeNormal()
{
  // Newell method
  Normal[0] = Normal[1] = Normal[2] = 0.0f;
  for (int i = 0; i < Count; i++) {
	  const osg::Vec3& v0 = (*Verts)[i].vertex;
	  const osg::Vec3& v1 = (*Verts)[(i + 1) % Count].vertex;
    Normal[0] += ((v0.y() - v1.y()) * (v0.z() + v1.z()));
    Normal[1] += ((v0.z() - v1.z()) * (v0.x() + v1.x()));
    Normal[2] += ((v0.x() - v1.x()) * (v0.y() + v1.y()));
  }

  // normalize
  Normal.normalize();

  if (Normal.length() == 0)
	  return false;
  else
	return true;
}


static inline bool isConvex(int w0, int w1, int w2)
{
  // caution: faces can fold around the normal
  const int v0 = WorkSet[w0];
  const int v1 = WorkSet[w1];
  const int v2 = WorkSet[w2];
  const osg::Vec3 e0 = (*Verts)[v1].vertex - (*Verts)[v0].vertex;
  const osg::Vec3 e1 = (*Verts)[v2].vertex - (*Verts)[v1].vertex;
  const osg::Vec3 cross = e0 ^ e1;
  if (cross * Normal <= 0.0f) {
    return false;
  }
  return true;
}


static inline bool isFaceClear(int w0, int w1, int w2)
{
  int i;
  const int v0 = WorkSet[w0];
  const int v1 = WorkSet[w1];
  const int v2 = WorkSet[w2];

  // setup the edges
  osg::Vec3 edges[3];
  edges[0] = (*Verts)[v1].vertex - (*Verts)[v0].vertex;
  edges[1] = (*Verts)[v2].vertex - (*Verts)[v1].vertex;
  edges[2] = (*Verts)[v0].vertex - (*Verts)[v2].vertex;

  // get the triangle normal
  osg::Vec3 normal = edges[0] ^ edges[1];

  // setup the planes
  osg::Vec4 planes[3];
  osg::Vec3 c0 = edges[0] ^ normal;
  planes[0].set( c0[0], c0[1], c0[2], -(c0 * (*Verts)[v0].vertex));
  osg::Vec3 c1 = edges[1] ^ normal;
  planes[0].set( c1[0], c1[1], c1[2], -(c1 * (*Verts)[v1].vertex));
  osg::Vec3 c2 = edges[2] ^ normal;
  planes[0].set( c2[0], c2[1], c2[2], -(c2 * (*Verts)[v2].vertex));

  for (int w = 0; w < Count; w++) {
    if ((w == w0) || (w == w1) || (w == w2)) {
      continue; // FIXME: lazy
    }
    const int v = WorkSet[w];
    for (i = 0; i < 3; i++) {
		const float dist = (*Verts)[v].vertex * osg::Vec3(planes[i][0], planes[i][1], planes[i][2]) + planes[i][3];
      if (dist > 0.0f) {
	break; // this point is clear
      }
    }
    if (i == 3) {
      return false;
    }
  }
  return true;
}


static inline float getDot(int w0, int w1, int w2)
{
  const int v0 = WorkSet[w0];
  const int v1 = WorkSet[w1];
  const int v2 = WorkSet[w2];
  osg::Vec3 e0 = (*Verts)[v1].vertex - (*Verts)[v0].vertex;
  osg::Vec3 e1 = (*Verts)[v2].vertex - (*Verts)[v1].vertex;
  e0.normalize();
  e1.normalize();
  return e0 * e1;
}


void mesh::triangulateFace(const vector<MeshVertex>& verts,
		     std::vector<TriIndices>& tris)
{
  tris.clear();

  Verts = &verts;
  Count = verts.size();
  WorkSet = new int[Count];
  for (int i = 0; i < Count; i++) {
    WorkSet[i] = i;
  }
  makeNormal();

  int best = 0;
  bool left = false;
  bool first = true;
  float score = 0.0f;

  while (Count >= 3) {
    bool convex = false;
    bool faceClear = false;

    int offset;
    if (best == Count) {
      offset = Count - 1;
    } else {
      offset = (best % Count);
    }

    // stripping pattern
    if (left) {
      offset = (offset + (Count - 1)) % Count;
    }
    left = !left;

    // find the best triangle
    for (int w = offset; w < offset + (Count - 2); w++) {
      const int w0 = (w + 0) % Count;
      const int w1 = (w + 1) % Count;
      const int w2 = (w + 2) % Count;

      const bool convex2 = isConvex(w0, w1, w2);
      if (convex && !convex2) {
	continue;
      }

      const bool faceClear2 = isFaceClear(w0, w1, w2);
      if ((faceClear && !faceClear2) && (convex || !convex2)) {
	continue;
      }

      if (first) {
	const float score2 = 2.0f - getDot(w0, w1, w2);
	if ((score2 < score) &&
	    (convex || !convex2) && (faceClear || !faceClear2)) {
	  continue;
	} else {
	  score = score2;
	}
      }

      best = w0;
      if (convex && faceClear) {
	break;
      }
      convex = convex2;
      faceClear = faceClear2;
    }

    first = false;

    // add the triangle
    TriIndices ti;
    ti.indices[0] = WorkSet[(best + 0) % Count];
    ti.indices[1] = WorkSet[(best + 1) % Count];
    ti.indices[2] = WorkSet[(best + 2) % Count];
    tris.push_back(ti);

    // remove the middle vertex
    const int m = (best + 1) % Count;
    memmove(WorkSet + m, WorkSet + m + 1, (Count - m - 1) * sizeof(int));
    Count--;
  }

  delete[] WorkSet;

  return;
}