#include "../../include/windows/eventHandlers/selectHandler.h"

selectHandler::selectHandler( View* view, osgGA::MatrixManipulator* manipulator ) : BZEventHandler( view ) {
	this->lastSelected = NULL;
	this->lastSelectedData = NULL;
	dx = dy = prev_x = prev_y = 0.0;
	this->cameraManipulator = manipulator;
}

// handle an event
bool selectHandler::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa ) {
	View* viewer;
	
    switch(ea.getEventType())
    {
    	
    	// catch drag events
    	case osgGA::GUIEventAdapter::DRAG :
    		viewer = dynamic_cast<View*>(&aa);
    		if( viewer && this->lastSelected && this->lastSelected->getName() == Selection_NODE_NAME ) {
    			if( this->prevEvent == osgGA::GUIEventAdapter::DRAG ) {
	    			this->dx = ea.getXnormalized() - this->prev_x;
	    			this->dy = ea.getYnormalized() - this->prev_y;
    			}
    			else {
    				this->dx = 0;
    				this->dy = 0;	
    			}
	    		this->prev_x = ea.getXnormalized();
    			this->prev_y = ea.getYnormalized();
    			
    			this->prevEvent = osgGA::GUIEventAdapter::DRAG;
    			return this->dragSelector( viewer, ea );
    		}
    		return false;
    		
    	// catch single-click events (see if we picked the selector
       	case osgGA::GUIEventAdapter::PUSH :
       		if( ea.getButton() == FL_LEFT_MOUSE ) {
       			viewer = dynamic_cast<View*>(&aa);
       			if( viewer ) {
       				this->prevEvent = osgGA::GUIEventAdapter::PUSH;
       				return this->pickSelector( viewer, ea );
       			}	
       		}
       		return false;
       		
    	// catch double click events and do a pick
        case osgGA::GUIEventAdapter::DOUBLECLICK :
        	if( ea.getButton() == FL_LEFT_MOUSE ) {
	            viewer = dynamic_cast<View*>(&aa);
	            if (viewer) {
	            	this->prevEvent = osgGA::GUIEventAdapter::DOUBLECLICK;
       		
	            	this->pickObject(viewer,ea);
	            }
        	}
       		return false;
       		
        default:
            return false;
    }
}

// use the OSG intersection API to pick objects
bool selectHandler::pickObject(View* viewer, const osgGA::GUIEventAdapter& ea) {
	// quit if the view is invalid
	if( viewer == NULL )
		return false;
		
	// intersections with the scene
    osgUtil::LineSegmentIntersector::Intersections intersections;
	
	// get the intersections from the point in the view where we clicked
    if(viewer->computeIntersections( ea.getX(), ea.getY(), intersections ) ) {
    	// iterate through the intersections
    	for(osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin(); hitr != intersections.end(); ++hitr) {
            
            if(hitr->nodePath.empty())
            	continue;
            
            for(unsigned int i = 0; i < hitr->nodePath.size(); i++) {
            	// only look for Renderables
            	Renderable* obj = dynamic_cast< Renderable* > ( hitr->nodePath[i] );
            	if(obj != NULL && obj->getName().length() > 0) {
            		if(!viewer->isPressed( FL_SHIFT )) {
            			viewer->unselectAll();
            		}
            		
					if(!viewer->isSelected( obj )) {
						viewer->setSelected( obj );	
					}
					else {
						viewer->setUnselected( obj );
					}
            		
            		// save the last selected object
            		this->lastSelected = obj;
            		
            		return true;
            	}
            }
            
        }
    }
    
    this->lastSelected = NULL;
    this->lastSelectedData = NULL;
    return false;
}

// use the OSG intersection API to pick objects
bool selectHandler::pickSelector(View* viewer, const osgGA::GUIEventAdapter& ea) {
	// quit if the view is invalid
	if( viewer == NULL )
		return false;
	
	// intersections with the scene
    osgUtil::LineSegmentIntersector::Intersections intersections;
	
	// get the intersections from the point in the view where we clicked
    if(viewer->computeIntersections( ea.getX(), ea.getY(), intersections ) ) {
    	// iterate through the intersections
    	for(osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin(); hitr != intersections.end(); ++hitr) {
            
            if(hitr->nodePath.empty())
            	continue;
            
            for(unsigned int i = 0; i < hitr->nodePath.size(); i++) {
            	// only look for Renderables
            	Selection* obj = dynamic_cast< Selection* > ( hitr->nodePath[i] );
            	if(obj != NULL) {
            		// determine which part of the selector got picked
        			osg::Node* pickedNode = Selection::getPickedNode( obj, hitr->nodePath, i+1 );
        			
            		// save the last selected object
            		this->lastSelected = obj;
            		
            		// save the node that was picked
            		this->lastSelectedData = pickedNode;
            		
            		return true;
            	}
            }
        }
    }
    
    this->lastSelected = NULL;
    this->lastSelectedData = NULL;
    return false;
}

// handle drag events
bool selectHandler::dragSelector( View* viewer, const osgGA::GUIEventAdapter& ea ) {
	osg::Node* node = (osg::Node*)this->lastSelectedData;
	
	// get the position of the last selected object (which should be the axes)
	osg::Vec3 position = this->lastSelected->getPosition();
	
	// transform the 2D mouse movement into a 3D vector by transforming it into camera space
	// get the vectors (but keep in mind that the window uses the XY-plane, but "up" in the 3D scene is along Z)
	osg::Vec3 sideVector = this->cameraManipulator->getSideVector( this->cameraManipulator->getMatrix() );
	osg::Vec3 upVector = this->cameraManipulator->getFrontVector( this->cameraManipulator->getMatrix() );
	
	// apply the transformation to each axis
	sideVector *= ( dx );
	upVector *= ( dy );
	
	// combine them into the transformation vector
	osg::Vec3 transformVector = sideVector + upVector;
	transformVector.normalize();
	
	
	if(node->getName() == Selection_X_AXIS_NODE_NAME) {
		// translate along x
		position.set( position.x() +  transformVector.x(), position.y(), position.z() );
	}
	else if(node->getName() == Selection_Y_AXIS_NODE_NAME) {
		// translate along y
		position.set( position.x(), position.y() + transformVector.y(), position.z() );	
	}
	else if(node->getName() == Selection_Z_AXIS_NODE_NAME) {
		// translate along z
		position.set( position.x(), position.y(), position.z() + transformVector.z() );	
	}
	
	// set the position
	Selection* selection = dynamic_cast< Selection* >( this->lastSelected );
	// update all objects in the selection
	if(selection) {
		map<Renderable*, Renderable*> selected = selection->getSelection();
		if( selected.size() > 0 ) {
			osg::Vec3 dPosition = position - selection->getPosition();
			for(map<Renderable*, Renderable*>::iterator i = selected.begin(); i != selected.end(); i++) {
				i->second->setPosition( i->second->getPosition() + dPosition );
				this->view->refresh( i->second );
			}	
		}
	}
	
	this->lastSelected->setPosition( position );
		
	return true;
}
