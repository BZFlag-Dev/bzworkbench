#include "../include/windows/View.h"

// view constructor
View::View(Model* m, int x, int y, int w, int h, const char *label) :
	RenderWindow(x,y,w,h) {
	
	// set OSG viewport
    this->getCamera()->setViewport(new osg::Viewport(0,0,w,h));
    
    // get the graphics context
    this->getCamera()->setGraphicsContext(this->getGraphicsWindow());
    
    // do single-threaded view
   this->setThreadingModel(osgViewer::Viewer::SingleThreaded);
   
   // set the model
   this->model = m;
   
   // initialize the root node
   this->root = new osg::Group();
   
   // initialize the ground
   this->initGround( 400.0f );
   
   // add the ground to the root node
   this->root->addChild( this->ground.get() );
	
	// set the key modifiers to false
   this->modifiers = map< int, bool >();
   this->modifiers[ FL_SHIFT ] = false;
   this->modifiers[ FL_CTRL ] = false;
   this->modifiers[ FL_ALT ] = false;
   this->modifiers[ FL_META ] = false;
   
   // set up the object map
   this->objectMap = map< bz2object*, osg::ref_ptr< Renderable > >();
	
	// build the scene from the model
	vector<bz2object*> objects = this->model->getObjects();
	if(objects.size() > 0) {
		for(vector<bz2object*>::iterator i = objects.begin(); i != objects.end(); i++) {
			osg::ref_ptr< Renderable > r = (*i)->makeRenderable();
			bool result = this->root->addChild( r.get() );
			if(result)
				this->objectMap[ *i ] = r;
		}
	}
	
	// add the root node to the scene
   this->setSceneData( this->root.get() );
   
   // give the View a trackball manipulator
   this->setCameraManipulator(new osgGA::TrackballManipulator());
   
   // add the stats event handler to the View
   this->addEventHandler(new osgViewer::StatsHandler());
   
   // make an event handler collection
   this->eventHandlers = new EventHandlerCollection( this );
   
   // add the selectHandler
   this->eventHandlers->addEventHandler( selectHandler::getName().c_str(), new selectHandler() );
   
   // add the scene picker event handler
   this->addEventHandler(eventHandlers.get());
   
}

// helper method: initialize the ground
void View::initGround( float size ) {
	
   // make a ground mesh (just a plane the size of the world)
   // ground points
   osg::Vec3Array* groundPoints = new osg::Vec3Array();
   groundPoints->push_back( osg::Vec3( -size, -size, 0 ) );
   groundPoints->push_back( osg::Vec3( size, -size, 0 ) );
   groundPoints->push_back( osg::Vec3( size, size, 0 ) );
   groundPoints->push_back( osg::Vec3( -size, size, 0 ) );
   
   // ground indexes
   osg::DrawElementsUInt* groundIndexes = new osg::DrawElementsUInt( osg::PrimitiveSet::QUADS, 0 );
   groundIndexes->push_back( 0 );
   groundIndexes->push_back( 3 );
   groundIndexes->push_back( 2 );
   groundIndexes->push_back( 1 );
   
   // texture coordinates
   osg::Vec2Array* groundTexCoords = new osg::Vec2Array();
   groundTexCoords->push_back( osg::Vec2(0.0, 0.0) );
   groundTexCoords->push_back( osg::Vec2(size, 0.0) );
   groundTexCoords->push_back( osg::Vec2(size, size) );
   groundTexCoords->push_back( osg::Vec2(0.0, size) );
   
   // make the member ground geode
   this->ground = SceneBuilder::renderable(
   					SceneBuilder::buildGeode( "ground", groundPoints, groundIndexes, groundTexCoords, "share/world/std_ground.png" ).get()
				  );
				  
}

// destructor
View::~View() { }


// draw method (really simple)
void View::draw(void) {
	this->frame();
}

// handle events
int View::handle(int event) {
	// whatever the event was, we need to regenerate the modifier keys
	int shiftState = Fl::event_state();
	this->modifiers[ FL_SHIFT ] = ( shiftState & FL_SHIFT );
	this->modifiers[ FL_CTRL ] = ( shiftState & FL_CTRL );
	this->modifiers[ FL_META ] = ( shiftState & FL_META );
	this->modifiers[ FL_ALT ] = ( shiftState & FL_ALT );
	
    // pass other events to the base class
	return RenderWindow::handle(event);
}

// update method (inherited from Observer)
void View::update( Observable* obs, void* data ) {
	// if data is not NULL, then we passed in an object to be updated
	if(data != NULL) {
		// get the bz2object
		bz2object* obj = (bz2object*)(data);
		
		// see if there is a renderable for this object
		if( this->objectMap.count( obj ) > 0 ) {
			// if so, update it
			obj->updateRenderable( this->objectMap[ obj ].get() );
		}
	}
	
	// refresh the scene
	this->redraw();
}

// handle Picker events
void View::handlePicker( BZEventHandler* picker, void* data ) {
	if(!data)
		return;
	
	Renderable* obj = (Renderable*)(data);
	
	// deselect everything upon picking a new selection
	// unless the SHIFT key is pressed
	if(!this->modifiers[ FL_SHIFT ])
		this->unselectAll();
	
	if(!isSelected( obj )) {
		this->setSelected( obj );	
	}
	else {
		this->setUnselected( obj );
	}

}

// is an object selected?
bool View::isSelected( Renderable* transformedNode ) {
	// a non-existant node isn't selected
	if(transformedNode == NULL)
		return false;
	
	return this->model->isSelected(transformedNode->getBZWObject());
}
/**
 * Set an object as selected (i.e. tints it green)
 */
void View::setSelected( bz2object* object ) {
	
	// tell the model that this object is to be selected
	model->setSelected( object );
	
}
void View::setSelected( Renderable* object ) { 
	
	if( object->getBZWObject() != NULL )	
		this->setSelected( object->getBZWObject() );
}

/**
 * This method does the same as setSelected, but instead of doing the selection, it undoes it.
 */
void View::setUnselected( bz2object* object ) {
	
	// tell the model that this is unselected
	model->setUnselected( object );
	
}
void View::setUnselected( Renderable* object ) { 
	
	if( object->getBZWObject() != NULL )
		this->setUnselected( object->getBZWObject() );
}


// mark all nodes unselected
void View::unselectAll() {
	this->model->unselectAll();
}
