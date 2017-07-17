/**
 * Creates a new settings object
 * @param {Axis} context The context of the settings
 * @constructor
 */
function AxSettings(context)
{
    AxResource.call(this);

    this.context = context;

    this.typeId = AxSettings.classId;
    
    this.resourceType = AxResourceType.Settings;
    
    this.properties.Clear();
    
    this.properties.Add(new AxProperty(new AxString("Clear screen"), true));
    this.properties.Add(new AxProperty(new AxString("Clear screen color"), new AxVector4(0.84, 0.92, 1.0, 1.0)));
    this.properties.Add(new AxProperty(new AxString("Ambient light"), new AxVector4(0.25, 0.25, 0.25, 1.0)));
    this.properties.Add(new AxProperty(new AxString("Thumbnail"), true));
    this.properties.Add(new AxProperty(new AxString("Thumbnail size"), 256));
    this.properties.Add(new AxProperty(new AxString("Embed generated meshes"), true));
    this.properties.Add(new AxProperty(new AxString("Embed generated textures"), false));
    this.properties.Add(new AxProperty(new AxString("Embed imported textures"), false));
    this.properties.Add(new AxProperty(new AxString("Embedded textures original encoding"), true));
}

AxSettings.prototype = Object.create(AxResource.prototype);


AxSettings.classId = (AxResourceType.Settings << 16) | 0;

AxSettings.propertyIndex_ClearScreen                        = 0;
AxSettings.propertyIndex_ClearScreenColor                   = 1;
AxSettings.propertyIndex_AmbientLight                       = 2;
AxSettings.propertyIndex_Thumbnail                          = 3;
AxSettings.propertyIndex_ThumbnailSize                      = 4;
AxSettings.propertyIndex_EmbedGeneratedMeshes               = 5;
AxSettings.propertyIndex_EmbedGeneratedTextures             = 6;
AxSettings.propertyIndex_EmbedImportedTextures              = 7;
AxSettings.propertyIndex_EmbeddedTexturesOriginalEncoding   = 8;

