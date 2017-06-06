/**
 * Creates an object with trace events
 * Trace events occur while tracing the scene in axis and can be used for rendering, picking and etc.
 * @param {Axis} context The instance of axis, which calls the events
 * @constructor
 */
function AxTraceEvents(context)
{
    this.context = context;
};

AxTraceEvents.prototype.Dispose = function()
{
};


// Virtual Methods

/**
 * Occurs when an entity of the respective type is encountered while tracing the scene
 * @param {AxTraceParameter} parameters holds the trace parameters, which are relevant at the time of the occurence of the event
 */

AxTraceEvents.prototype.OnMesh = function(parameters) { };
AxTraceEvents.prototype.OnCamera = function(parameters) { };
AxTraceEvents.prototype.OnTransform = function(parameters) { };
AxTraceEvents.prototype.OnRenderTarget = function(parameters) { };
AxTraceEvents.prototype.OnMaterial = function(parameters) { };
AxTraceEvents.prototype.OnMechanism = function(parameters) { };
AxTraceEvents.prototype.OnLight = function(parameters) { };
AxTraceEvents.prototype.OnSoundEmitter = function(parameters) { };
AxTraceEvents.prototype.OnSoundReceptor = function(parameters) { };
AxTraceEvents.prototype.OnTexture2D = function(parameters) { };

AxTraceEvents.prototype.OnExitEntitySet = function(parameters) { };