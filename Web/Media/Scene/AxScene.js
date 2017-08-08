/**
 * Creates a new scene media object
 * This is a virtual prototype meant to be inherited by overrideing its methods
 * @constructor
 */
function AxScene()
{
    AxMedia.call(this);

    this.context = null;
}

AxScene.prototype = Object.create(AxMedia.prototype);



/**
 * Sets the context for the scene media.
 * The context is needed to perform all loading, saving, of resources, reading and writing of data by the scene media.
 * @param {Axis} context The context to be set. It's an instance of axis, which the scene media will work with
 */
AxScene.prototype.SetContext = function(context)
{
    this.context = context;
};
