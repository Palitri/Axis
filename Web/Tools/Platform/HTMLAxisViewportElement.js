/**
 * Provides the <axis-viewport> html element for the Axis framework
 */
class HTMLAxisViewportElement extends HTMLElement 
{
    constructor() 
    {
        super();

        this.viewport = null;
        this.axis = null;
        this.canvas = null;
        
        this.srcAttribute = null;
        this.onInitializeAttribute = null;
        this.onLoadAttribute = null;
        this.onRenderAttribute = null;
        this.scrollingEnabledAttribute = false;
        this.contextMenuEnabledAttribute = false;
        this.defaultControlsAttribute = false;
        this.orbitAttribute = false;
        this.rotateAttribute = false;
        this.panAttribute = false;
        this.zoomAttribute = false;
    }
    
    /**
     * Inherited static property, which returns an array of names of these of the object's properties, for which the attributeChangedCallback method is to fire whenever their value changes
     */
    static get observedAttributes()
    {
        return ["src"];
    }

    
    /**
     * Inherited mehtod which fires when the html element is 
     */
    connectedCallback() 
    {
        QueryDocumentLoadedCallback(this, function(instance)
        {
            instance.Initialize();
        });    
    }
    
    /**
     * Fires when an attribute, whose name is in the retured values of observedAttributes, changes.
     */
    attributeChangedCallback(name, oldValue, newValue) 
    {
        var instance = this;
        
        if (name === "src")
        {
            // May be a string, but also an expression such as getModel(), which returns an AxStream, so it has to be evaluated and evaluated when both the model and Axis are loaded
            instance.srcAttribute = this.src;
            try
            {
                instance.srcAttribute = eval(instance.srcAttribute);
            }
            catch(e)
            {
            }
            
            instance.LoadSrc();
        }
    }

    /**
     * 
     * @param {String} attributeName Name of the attribute to be created
     * @param {*} defaultValue Default value to use if an attribute with the given name does not exist
     * @param {function} selector A function which returns a useful object base on the contents of the attribute
     */
    CreateAttribute(attributeName, defaultValue, selector)
    {
        if (this.hasAttribute(attributeName))
        {
            var attribute = this.getAttribute(attributeName);
            
            if (selector !== "undefined")
                return selector(attribute);
            
            return attribute;
        }
        
        return defaultValue;
    }

    CallAttributeFunction(callback)
    {
        if (callback !== null)
          {
              try
              {
                  var instance = this;
                  callback();
              }
              catch(e)
              {
              }
          }
    }

    /**
     * Creates any relevant html content and initializes the html element
     */
    Initialize()
    {
        this.style.display = "block";

        this.canvas = document.createElement("canvas");
        this.canvas.style.width = "100%";
        this.canvas.style.height = "100%";
        this.appendChild(this.canvas);

        this.onInitializeAttribute = this.CreateAttribute("oninitialize", null, a => new Function(a));
        this.onLoadAttribute = this.CreateAttribute("onload", null, a => new Function(a));
        this.onRenderAttribute = this.CreateAttribute("onrender", null, a => new Function(a));
        this.scrollingEnabledAttribute = this.CreateAttribute("scrollingenabled", false, a => a == "true");
        this.contextMenuEnabledAttribute = this.CreateAttribute("contextmenuenabled", false, a => a == "true");
        this.defaultControlsAttribute = this.CreateAttribute("defaultcontrols", false, a => a == "true");
        if (this.defaultControlsAttribute)
        {
            this.orbitAttribute = true;
            this.rotateAttribute = false;
            this.panAttribute = true;
            this.zoomAttribute = true;
        }
        this.orbitAttribute = this.CreateAttribute("orbit", this.orbitAttribute, a => a == "true");
        this.rotateAttribute = this.CreateAttribute("rotate", this.rotateAttribute, a => a == "true");
        this.panAttribute = this.CreateAttribute("pan", this.panAttribute, a => a == "true");
        this.zoomAttribute = this.CreateAttribute("zoom", this.zoomAttribute, a => a == "true");

        this.viewport = new AxAnimationElement(this.canvas);
        this.viewport.touchScrollingEnabled = this.scrollingEnabledAttribute;
        this.viewport.contextMenuEnabled = this.contextMenuEnabledAttribute;
        
        this.axis = new Axis(this.viewport.canvas);

        var instance = this;
        this.viewport.OnRender = function() 
        { 
            instance.axis.RenderScene();
            
            instance.SetupAttributeControls();

            instance.CallAttributeFunction(instance.onRenderAttribute);
        };
        
        this.CallAttributeFunction(this.onInitializeAttribute);
        
        this.LoadSrc();
    }
    
    /**
     * Loads the scene from the src attribute of the html element
     */
    LoadSrc()
    {
        if (this.axis === null)
            return;
        
        if (this.srcAttribute)
        {
            var instance = this;
            this.axis.Reset();
            this.axis.ImportScene(this.srcAttribute, function(context, fileName, success) 
            { 
                instance.firstFrame = true;
                instance.CallAttributeFunction(instance.onLoadAttribute);
                instance.viewport.StartRendering();
            });
        }
    }
    
    /**
     * Sets up the input control models selected via the relevant properties of the html element
     */
    SetupAttributeControls()
    {
        if (this.firstFrame)
        {
            var camera = this.axis.currentCamera;
            var translationFactor = AxMaths.SizeFromDistanceAndAngle(camera.transformRef.GetPositionExtrinsic().GetLength(), camera.cameraRef.properties.Get(AxCamera.propertyIndex_FOV).GetFloat());
            
            if (this.orbitAttribute)
            {
                var pivot = new AxTransform();
                this.axis.inputModels.Add(new AxOrbitInputModel(this.axis, pivot, camera.transformRef, 1.0, 6.0,
                    "", "Pointer Y", "Pointer X", "", "", "", "Pointer Pressed"));
            }
            else if (this.rotateAttribute)
            {
                this.axis.inputModels.Add(new AxWalkInputModel(this.axis, camera.transformRef, translationFactor, 6.0, 1.0, 6.0,
                    "", "", "", "", "", "", "", "Pointer Y", "", "", "Pointer X", "Pointer Pressed"));
            }
            
            if (this.zoomAttribute)
            {
                this.axis.inputModels.Add(new AxWalkInputModel(this.axis, camera.transformRef, translationFactor / 4.0, 6.0, 1.0, 6.0,
                    "Mouse Scroll", "", "", "", "", "", "", "", "", "", "", ""));

                this.axis.inputModels.Add(new AxWalkInputModel(this.axis, camera.transformRef, translationFactor * 4.0, 6.0, 1.0, 6.0,
                    "Mouse Y", "", "", "", "", "", "Mouse Middle", "", "", "", "", ""));
            }
            
            if (this.panAttribute)
            {
                this.axis.inputModels.Add(new AxWalkInputModel(this.axis, camera.transformRef, translationFactor, 6.0, 1.0, 6.0,
                    "", "", "Mouse X", "", "", "Mouse Y", "Mouse Right", "", "", "", "", ""));
            }
            
            this.firstFrame = false;
        }
    }
    
    get src() { return this.getAttribute("src"); }
    set src(value) { this.setAttribute("src", value); }
}


customElements.define('axis-viewport', HTMLAxisViewportElement);


/**
 * Invokes a callback function when the document has finished loading.
 * If document has already finished loading at the time of calling this function, the callback gets invoked immediately.
 * @param {*} parameter A parameter which is to be passed as an argument when invoking the callback function
 * @param {*} callback The callback function to invoke when the document has finished loading.
 */
function QueryDocumentLoadedCallback(parameter, callback)
{
    if (document.readyState === "complete")
        callback(parameter);
    else
    {
        document.onreadystatechange = function () 
        {
            if (document.readyState === "complete") 
                callback(parameter);
        };
    }
}
