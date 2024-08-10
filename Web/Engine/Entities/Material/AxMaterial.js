/**
 * Creates a new material
 * @param {Axis} context The context to use for the material
 * @constructor
 */
function AxMaterial(context)
{
    AxResource.call(this);
    
    this.context = context;

    this.typeId = AxMaterial.classId;
    this.resourceType = AxResourceType.Material;

    this.properties.Add(new AxProperty(new AxString('Override Materials'), false));

    this.deviceShader = this.context.graphicsDevice.CreateShader();

    this.shadingLayers = new AxList();
    this.renderStates = new AxList();

    this.lightSourcesCount = 0;
    this.isAnimated = false;
    this.successfullyCompiled = false;
    this.compilationMessage = new AxString();

    this.paramAddressWorldMatrix = -1;
    this.paramAddressViewMatrix = -1;
    this.paramAddressProjectionMatrix = -1;
    this.paramAddressViewPosition = -1;
    this.paramAddressLightsPos = -1;
    this.paramAddressLightsColor = -1;
    this.paramAddressLightsIntensity = -1;
    this.paramAddressActiveLightsCount = -1;
    this.paramAddressAmbientLight = -1;
    
    this.shadingPropertiesAddresses = new AxList();
    
    this.noCullingPropertyIndex = -1;

    this.updateState = false;

    this.CommitRenderStates();
}

AxMaterial.prototype = Object.create(AxResource.prototype);


AxMaterial.SerializationId_ShadingLayers		= 0x21111003;
AxMaterial.SerializationId_ShadingLayer		= 0x21111103;
AxMaterial.SerializationId_RenderStates		= 0x21112003;
AxMaterial.SerializationId_RenderState		= 0x21112103;

AxMaterial.classId = (AxResourceType.Material << 16) | 0;

AxMaterial.propertyIndex_OverrideMaterials = AxResource.propertyIndex_ChildPropertiesIndex + 0;


AxMaterial.prototype.Dispose = function()
{
};


/*
 * Creates a property name by combining the given desiredName and propertyNumber.
 * Using this method is recommended when it's needed to create multiple properties of the same name.
 * The desiredName and the propertyNumber are combined by concatenating them with a space in between. If propertyNumber is zero, the result is the unchanged desiredName.
 * @param {AxString} desiredName The desirable property name
 * @param {Integer} propertyNumber The number to combine with the given desiredName. If zero, the return value would be the unchanged desiredName
 * @returns {AxString} A name, created by combining the given desiredName and propertyNumber
 */
AxMaterial.CreatePropertyName = function(desiredName, propertyNumber)
{
    desiredName = AxString.GetAxString(desiredName);

    if (propertyNumber === 0)
        return new AxString(desiredName.contents);

    var result = new AxString(desiredName.contents + " " + propertyNumber);
    return new AxString(result);
};

/*
 * Creates a unique property name, based on the given desiredName.
 * If a property of the given desiredName doesn't already exist, returns desiredName unchanged. Otherwise, returns the given desiredName, combined with the smallest number which will produce a unique name.
 * @param {AxString} desiredName The desirable property name
 * @returns {AxString} A unique name for a property, based on the given desiredName
 */
AxMaterial.prototype.CreateUniquePropertyName = function(desiredName)
{
    desiredName = AxString.GetAxString(desiredName);
    
    var result = new AxString(desiredName.contents);
    var propertyNumber = 0;

    while (this.GetProperty(result) !== null)
    {
        propertyNumber++;
        result = AxMaterial.CreatePropertyName(desiredName, propertyNumber);
    }

    return new AxString(result);
};

/*
 * Sets a property by taking the value of the given one.
 * If a property with a name as the given one exists, its value will be updated, otherwise a new property will be created
 * @param {Integer} index The index at whitch to insert a new property in case one is to be created
 * @param {AxProperty} prop The property which is to be set
 * @returns {Boolean} True if a new property was created, false if an existing property with the same name was updated
 */
AxMaterial.prototype.SetProperty = function(index, prop)
{
    var existingProp = this.GetProperty(prop.name);
    if (existingProp === null)
    {
        existingProp = new AxProperty(prop.name, prop.value, prop.type);
        this.properties.Insert(index, existingProp);
        return true;
    }
    else
    {
        existingProp.SetValue(prop.value, prop.type);
        return false;
    }
};

/*
 * Inserts the proper property for the value of a given color operation
 * @param {Integer} index The index at which to insert the property
 * @param {AxString} name The name of the property to insert
 * @param {AxColorOperation} colorOp The color operation for which to insert the property
 */
AxMaterial.prototype.InsertColorOperationProperty = function(index, name, colorOp)
{
    switch (colorOp)
    {
        case AxColorOperation.None:
        {
            break;
        }

        case AxColorOperation.Set:
        case AxColorOperation.Blend:
        case AxColorOperation.Add:
        case AxColorOperation.Subtract:
        case AxColorOperation.Modulate:
        case AxColorOperation.Lighter:
        case AxColorOperation.Darker:
        case AxColorOperation.AlphaBlend:
        case AxColorOperation.AlphaAdd:
        case AxColorOperation.AlphaSubtract:
        case AxColorOperation.AlphaModulate:
        {
            this.properties.Insert(index, new AxProperty(name, 1.0));
            break;
        }
    }
};

/**
 * Applies the material's render states
 */
AxMaterial.prototype.CommitRenderStates = function()
{
    var numRenderStates = this.renderStates.count;
    for (var i = 0; i < numRenderStates; i++)
        this.context.graphicsDevice.SetRenderState(this.renderStates.Get(i));
};

/**
 * Inserts a shading layer at a given position
 * @param {AxShadingOperation} shadingOp The shading operation to insert
 * @param {AxColorOperation} colorOp The color operation to use with the shading operation
 * @param {Integer} index The index at which to insert the shading operation
 */
AxMaterial.prototype.InsertShadingLayer = function(shadingOp, colorOp, index)
{
    var numProperties = this.properties.count;

    var propertyIndex = index < this.shadingLayers.count ? this.shadingLayers.Get(index).propertyFirst : numProperties;
    var propertyFirst = propertyIndex;

    var defaultTexture = null;//this.context.defaultTexture;

    var countRecurringOps = this.GetShadingLayersCount(shadingOp);

    switch (shadingOp)
    {
        case AxShadingOperation.BasicShading:
        {
            break;
        }

        case AxShadingOperation.FlatColor:
        {
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("Flat color", countRecurringOps), new AxVector4()));

            this.InsertColorOperationProperty(propertyIndex++, AxMaterial.CreatePropertyName("Flat color amount", countRecurringOps), colorOp);
            break;
        }

        case AxShadingOperation.Tint:
        {
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("Tint color", countRecurringOps), new AxVector4()));

            this.InsertColorOperationProperty(propertyIndex++, AxMaterial.CreatePropertyName("Tint amount", countRecurringOps), colorOp);
            break;
        }

        case AxShadingOperation.AmbientLighting:
        {
            this.InsertColorOperationProperty(propertyIndex++, AxMaterial.CreatePropertyName("Ambient amount", countRecurringOps), colorOp);
            break;
        }

        case AxShadingOperation.LightSource:
        {
            break;
        }

        case AxShadingOperation.VertexLighting:
        {
            if (this.SetProperty(propertyIndex, new AxProperty(AxMaterial.CreatePropertyName("Diffuse", countRecurringOps), new AxVector4(0.5, 0.5, 0.5, 1.0))))
                propertyIndex++;
            if (this.SetProperty(propertyIndex, new AxProperty(AxMaterial.CreatePropertyName("Specular", countRecurringOps), new AxVector4(1.0, 1.0, 1.0, 1.0))))
                propertyIndex++;
            if (this.SetProperty(propertyIndex, new AxProperty(AxMaterial.CreatePropertyName("Gloss", countRecurringOps), 10.0)))
                propertyIndex++;

            this.InsertColorOperationProperty(propertyIndex++, AxMaterial.CreatePropertyName("Vertex lighting amount", countRecurringOps), colorOp);
            break;
        }

        case AxShadingOperation.PixelLighting:
        {
            if (this.SetProperty(propertyIndex, new AxProperty(AxMaterial.CreatePropertyName("Diffuse", countRecurringOps), new AxVector4(0.5, 0.5, 0.5, 1.0))))
                propertyIndex++;
            if (this.SetProperty(propertyIndex, new AxProperty(AxMaterial.CreatePropertyName("Specular", countRecurringOps), new AxVector4(1.0, 1.0, 1.0, 1.0))))
                propertyIndex++;
            if (this.SetProperty(propertyIndex, new AxProperty(AxMaterial.CreatePropertyName("Gloss", countRecurringOps), 10.0)))
                propertyIndex++;

            this.InsertColorOperationProperty(propertyIndex++, AxMaterial.CreatePropertyName("Pixel lighting amount", countRecurringOps), colorOp);
            break;
        }

        case AxShadingOperation.ColorMap:
        {
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("Color map", countRecurringOps), defaultTexture, AxParameterType.ReferenceTexture));

            this.InsertColorOperationProperty(propertyIndex++, AxMaterial.CreatePropertyName("Color map amount", countRecurringOps), colorOp);
            break;
        }

        case AxShadingOperation.ReflectionMap:
        {
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("Reflection map", countRecurringOps), defaultTexture, AxParameterType.ReferenceTexture));
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("Reflection map factor", countRecurringOps), 1.0));

            this.InsertColorOperationProperty(propertyIndex++, AxMaterial.CreatePropertyName("Reflection map amount", countRecurringOps), colorOp);
            break;
        }


        case AxShadingOperation.RefractionMap:
        {
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("Refraction map", countRecurringOps), defaultTexture, AxParameterType.ReferenceTexture));
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("Refraction map factor", countRecurringOps), 0.0));

            this.InsertColorOperationProperty(propertyIndex++, AxMaterial.CreatePropertyName("Refraction map amount", countRecurringOps), colorOp);
            break;
        }

        case AxShadingOperation.NormalMap:
        {
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("Normal map", countRecurringOps), defaultTexture, AxParameterType.ReferenceTexture));
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("Normal map factor", countRecurringOps), 1.0));
            break;
        }

        case AxShadingOperation.BlurMap:
        {
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("Blur map", countRecurringOps), defaultTexture, AxParameterType.ReferenceTexture));
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("Blur map factor", countRecurringOps), 0.0));

            this.InsertColorOperationProperty(propertyIndex++, AxMaterial.CreatePropertyName("Blur map amount", countRecurringOps), colorOp);
            break;
        }

        case AxShadingOperation.HBlur:
        {
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("HBlur map", countRecurringOps), defaultTexture, AxParameterType.ReferenceTexture));
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("HBlur map factor", countRecurringOps), 0.0));

            this.InsertColorOperationProperty(propertyIndex++, AxMaterial.CreatePropertyName("HBlur map amount", countRecurringOps), colorOp);
            break;
        }

        case AxShadingOperation.VBlur:
        {
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("VBlur map", countRecurringOps), defaultTexture, AxParameterType.ReferenceTexture));
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("VBlur map factor", countRecurringOps), 0.0));

            this.InsertColorOperationProperty(propertyIndex++, AxMaterial.CreatePropertyName("VBlur map amount", countRecurringOps), colorOp);
            break;
        }

        case AxShadingOperation.AlignScreenQuad:
        {
            break;
        }

        case AxShadingOperation.ProjectTexture:
        {
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("Projection Map", countRecurringOps), defaultTexture, AxParameterType.ReferenceTexture));
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("Projection Cam", countRecurringOps), 0, AxParameterType.ReferenceCamera));

            this.InsertColorOperationProperty(propertyIndex++, AxMaterial.CreatePropertyName("Project texture amount", countRecurringOps), colorOp);
            break;
        }

        case AxShadingOperation.ProjectPicture:
        {
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("Picture Map", countRecurringOps), defaultTexture, AxParameterType.ReferenceTexture));
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("Picture Cam", countRecurringOps), 0, AxParameterType.ReferenceCamera));

            this.InsertColorOperationProperty(propertyIndex++, AxMaterial.CreatePropertyName("Project picture amount", countRecurringOps), colorOp);
            break;
        }

        case AxShadingOperation.ShadowMapWriter:
        {
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("Shadow map depth", countRecurringOps), 0.0));
            break;
        }

        case AxShadingOperation.ShadowMap:
        {
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("ShadowMap Cam", countRecurringOps), 0, AxParameterType.ReferenceCamera));
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("Shadow Map", countRecurringOps), defaultTexture, AxParameterType.ReferenceTexture));
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("Light Map", countRecurringOps), defaultTexture, AxParameterType.ReferenceTexture));
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("Bias", countRecurringOps), 0.0));
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("Shadow map depth", countRecurringOps), 0.0));
            break;
        }

        case AxShadingOperation.TextureTransform:
        {
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("Texture Transform", countRecurringOps), 0, AxParameterType.ReferenceTransform));
            break;
        }

        // TODO: Consider removing this operation. Currently, it only provides the properties without actually doing anything
        case AxShadingOperation.Material:
        {
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("Ambient", countRecurringOps), new AxVector4(0.2, 0.2, 0.2, 1.0)));
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("Diffuse", countRecurringOps), new AxVector4(0.5, 0.5, 0.5, 1.0)));
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("Specular", countRecurringOps), new AxVector4(1.0, 1.0, 1.0, 1.0)));
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("Gloss", countRecurringOps), 10.0));
            
            break;
        }

        case AxShadingOperation.Fog:
        {
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("Fog color", countRecurringOps), new AxVector4(0.8, 0.8, 0.8, 1.0)));
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("Fog start", countRecurringOps), 100.0));
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("Fog density", countRecurringOps), 100.0));

            if (this.InsertColorOperationProperty(propertyIndex, AxMaterial.CreatePropertyName("Fog amount", countRecurringOps), colorOp))
                propertyIndex++;

            break;
        }
        // TODO: consider moving this to a "Parameters" resource entity, dedicated to changing various parameters of the engine alont traversing the scene
        case AxShadingOperation.ChangeCulling:
        {
            this.properties.Insert(propertyIndex++, new AxProperty(AxMaterial.CreatePropertyName("No culling", countRecurringOps), false));

            break;
        }
    }

    var numNewProperties = this.properties.count - numProperties;
    this.shadingLayers.Insert(index, new AxMaterialShadingLayer(shadingOp, colorOp, propertyFirst, numNewProperties));

    var count = this.shadingLayers.count;
    for (var i = index + 1; i < count; i++)
        this.shadingLayers.Get(i).propertyFirst += numNewProperties;
};

/**
 * Adds a shading layer to the material
 * @param {AxShadingOperation} shadingOp The shading operation to insert
 * @param {AxColorOperation} colorOp The color operation to use with the shading operation
 * @return {Integer} The index at which the added shading layer can be found. Practically, this is the last index in the material's shading layers collection
 */
AxMaterial.prototype.AddShadingLayer = function(shadingOp, colorOp)
{
    this.InsertShadingLayer(shadingOp, colorOp, this.shadingLayers.count);
    return this.shadingLayers.count - 1;
};

/**
 * Removes a shading layer at specified location
 * @param {Integer} index The index of the shading layer to remove
 */
AxMaterial.prototype.RemoveShadingLayer = function(index)
{
    var count = this.shadingLayers.count;
    for (var i = index + 1; i < count; i++)
        this.shadingLayers.Get(i).propertyFirst -= this.shadingLayers.Get(index).propertyCount;

    this.properties.RemoveRange(this.shadingLayers.Get(index).propertyFirst, this.shadingLayers.Get(index).propertyCount);
    this.shadingLayers.RemoveAt(index);
};

/**
 * Moves a shading layer from one position to another in the material's shading layers collection
 * @param {Integer} originalIndex The original index of the shading layer which is to be moved
 * @param {Integer} newIndex The index to which the shading layer should be moved to
 */
AxMaterial.prototype.MoveShadingLayer = function(originalIndex, newIndex)
{
    var originalIndexPropertiesStart = this.shadingLayers.Get(originalIndex).propertyFirst;
    var originalIndexPropertiesCount = this.shadingLayers.Get(originalIndex).propertyCount;

    var start = AxMath.Min(originalIndex, newIndex);
    var propertyFirst = this.shadingLayers.Get(start).propertyFirst;

    this.shadingLayers.Move(originalIndex, newIndex, 1);

    var count = this.shadingLayers.count;
    for (var i = start; i < count; i++)
    {
        this.shadingLayers.Get(i).propertyFirst = propertyFirst;
        propertyFirst += this.shadingLayers.Get(i).propertyCount;
    }

    this.properties.Move(originalIndexPropertiesStart, this.shadingLayers.Get(newIndex).propertyFirst, originalIndexPropertiesCount);
};

/**
 * Removes all shading layers from the material
 */
AxMaterial.prototype.ClearShadingLayers = function()
{
    this.shadingLayers.Clear();
    this.properties.Clear();

    this.shadingProperties.Clear();
};

/*
 * Returns the number of shading layers of a specified signature
 * @param {AxShadingOperation} shadingOp The shading operation of the layer
 * @param {!AxColorOperation} colorOp The color operation of the layer. If omitted, the color operaion will be disregarded.
 * @returns {Integer} The number of shading layers with the specified signature
 */
AxMaterial.prototype.GetShadingLayersCount = function(shadingOp, colorOp)
{
    var result = 0;
    var ignoreColorOp = AxUtils.IsUndefinedOrNull(colorOp);
    for (var i = 0; i < this.shadingLayers.count; i++)
    {
        var shadingLayer = this.shadingLayers.Get(i);
        if ((shadingLayer.shadingOp === shadingOp) && (ignoreColorOp || (shadingLayer.colorOp === colorOp)))
            result++;
    }

    return result;
};

/**
 * Updates the material shader according to the material's shading layers
 * @return {Boolean} True if the shader was successfully updated
 */
AxMaterial.prototype.UpdateShader = function()
{
    this.successfullyCompiled = false;

    // Setup material
    this.deviceShader.ClearOperations();
    for (var i = 0; i < this.shadingLayers.count; i++)
        this.deviceShader.AddOperation(this.shadingLayers.Get(i).shadingOp, this.shadingLayers.Get(i).colorOp);

    // Update shader
    if (!this.deviceShader.Update(this.compilationMessage))
        return false;

    // Setup parameters
    this.shadingPropertiesAddresses.SetSize(this.properties.count);
    for (var i = 0; i < this.properties.count; i++)
    {
        this.shadingPropertiesAddresses.Set(i, this.deviceShader.GetParameterAddress(this.properties.Get(i).name));
    }

    this.noCullingPropertyIndex = this.properties.IndexOf(this.GetProperty("No culling"));

    this.paramAddressWorldMatrix = this.deviceShader.GetParameterAddress(new AxString('World matrix'));
    this.paramAddressViewMatrix = this.deviceShader.GetParameterAddress(new AxString('View matrix'));
    this.paramAddressProjectionMatrix = this.deviceShader.GetParameterAddress(new AxString('Projection matrix'));
    this.paramAddressViewPosition = this.deviceShader.GetParameterAddress(new AxString('View position'));
    this.paramAddressLightsPos = this.deviceShader.GetParameterAddress(new AxString('Lights pos'));
    this.paramAddressLightsColor = this.deviceShader.GetParameterAddress(new AxString('Lights color'));
    this.paramAddressLightsIntensity = this.deviceShader.GetParameterAddress(new AxString('Lights intensity'));
    this.paramAddressActiveLightsCount = this.deviceShader.GetParameterAddress(new AxString('Active lights count'));
    this.paramAddressAmbientLight = this.deviceShader.GetParameterAddress(new AxString('Ambient light'));

    // Get some common parameters
    this.isAnimated = false;
    this.lightSourcesCount = 0;
    for (var i = 0; i < this.shadingLayers.count; i++)
    {
        if (this.shadingLayers.Get(i).shadingOp === AxShadingOperation.LightSource)
            this.lightSourcesCount++;
        if (this.shadingLayers.Get(i).shadingOp === AxShadingOperation.Skinning)
            this.isAnimated = true;
    }

    this.successfullyCompiled = true;
    return true;
};

/**
 * Applies a material property's value to the shader
 * @param {AxProperty} materialProperty The prperty, whose value is to be applied to the shader
 * @param {Integer} shaderAddress The address of the property in terms of the material's shader
 */
AxMaterial.prototype.ApplyProperty = function(materialProperty, shaderAddress)
{
    if (!this.successfullyCompiled)
        return;

    switch (materialProperty.type)
    {
        case AxParameterType.ReferenceTexture:
        {
            var texture = materialProperty.GetEffectiveValue();
            if (texture !== null)
                this.deviceShader.SetParameter(shaderAddress, 0, texture.deviceTexture);
            break;
        }

        case AxParameterType.ReferenceCamera:
        {
            var camera = materialProperty.GetEffectiveValue();
            if (camera !== null)
                this.deviceShader.SetParameter(shaderAddress, 0, camera.viewProjection);
            break;
        }

        case AxParameterType.ReferenceTransform:
        {
            var transform = materialProperty.GetEffectiveValue();
            if (transform !== null)
                this.deviceShader.SetParameter(shaderAddress, 0, transform.localMatrix);
            break;
        }

        case AxParameterType.ReferenceVector3:
        {
            var transform = materialProperty.GetEffectiveValue();
            if (transform !== null)
            {
                this.deviceShader.SetParameter(shaderAddress, 0, transform.localMatrix._41);
            }
            break;
        }

        default:
        {
            this.deviceShader.SetParameter(shaderAddress, 0, materialProperty.GetEffectiveValue());
            break;
        }
    }
};

/**
 * Applies all material property values to the shader
 */
AxMaterial.prototype.ApplyProperties = function()
{
    if (!this.successfullyCompiled)
        return;

    if (this.properties.count !== this.shadingPropertiesAddresses.count)
        return;

    var count = this.properties.count;
    for (var i = 0; i < count; i++)
        this.ApplyProperty(this.properties.Get(i), this.shadingPropertiesAddresses.Get(i));

    if (this.noCullingPropertyIndex !== -1)
        if (this.properties.Get(this.noCullingPropertyIndex).GetBool())
            this.context.graphicsDevice.SetRenderState(AxRenderState.CullNone);
        else
            this.context.graphicsDevice.SetRenderState(AxRenderState.CullFrontFace);
    else
        this.context.graphicsDevice.SetRenderState(AxRenderState.CullBackFace);
};

/**
 * If the material has light sources, sets them, choosing from the lights in the scene, which are closest to the given location
 * The location should generally be the place where the object rendered with this material is located in the scene
 * @param {AxVector3} location Location in relation to which to select the closest lights in the scene
 */
AxMaterial.prototype.ApplyLights = function(location)
{
    //*
    if (this.lightSourcesCount > 0)
    {
        this.context.CalculateNearestActiveLights(this.lightSourcesCount, location);
        var lightsCount = this.context.nearestActiveLights.count;
        this.deviceShader.SetParameter(this.paramAddressActiveLightsCount, 0, lightsCount);
        if (lightsCount > 0)
        {
            if ((this.paramAddressLightsPos !== -1) && (this.paramAddressLightsColor !== -1) && (this.paramAddressLightsIntensity !== -1))
            {
                for (var i = 0; i < lightsCount; i++)
                {
                    this.deviceShader.SetParameter(this.paramAddressLightsPos, i, this.context.nearestActiveLights.Get(i).lightPos);
                    this.deviceShader.SetParameter(this.paramAddressLightsColor, i , this.context.nearestActiveLights.Get(i).lightRef.properties.Get(AxLight.propertyIndex_Color).GetEffectiveValue());
                    this.deviceShader.SetParameter(this.paramAddressLightsIntensity, i , this.context.nearestActiveLights.Get(i).lightRef.properties.Get(AxLight.propertyIndex_Intensity).GetEffectiveValue());
                }
            }
        }
    }
    else
        this.deviceShader.SetParameter(this.paramAddressActiveLightsCount, 0, this.lightSourcesCount);
    //*/
};

/**
 * Sets the context's current bone transformations to the material's shader
 */
AxMaterial.prototype.ApplyBones = function()
{
    // TODO: addresses of parameters should be precalculated, or GetParameterAddress should accept not a parameter name, but an id, so it can map fast and directly to the address
    if (this.isAnimated)
    {
        if (this.paramAddressWorldMatrix !== -1)
        {
            for (var i = 0; i < this.context.bones.count; i++)
            {
                this.deviceShader.SetParameter(this.paramAddressWorldMatrix ,i, this.context.bones.Get(i));
            }
        }
    }
};


/**
 * Deserializes the material from a given stream
 * @param {AxStream} source The stream holding the serialized data
 * @return {Boolean} True if the deserialization was successfull
 */
AxMaterial.prototype.Deserialize = function(source)
{
    if (!AxResource.prototype.Deserialize.call(this, source))
        return false;
    
    return this.UpdateShader();
};

/**
 * Writes chunks for all the data which is needed to serialize the material. 
 * @param {AxHierarchyStreamWriter} writer Writer to use for writing the serialization data
 */
AxMaterial.prototype.SerializeChunks = function(writer)
{
    writer.BeginChunk(AxMaterial.SerializationId_ShadingLayers);

    for (var i = 0; i < this.shadingLayers.count; i++)
    {
        writer.BeginChunk(AxMaterial.SerializationId_ShadingLayer);
        writer.stream.WriteInt16(this.shadingLayers.Get(i).shadingOp);
        writer.stream.WriteInt16(this.shadingLayers.Get(i).colorOp);

        writer.EndChunk();
    }

    writer.EndChunk();

    writer.BeginChunk(AxMaterial.SerializationId_RenderStates);

    for (var i = 0; i < this.shadingLayers.count; i++)
    {
        writer.BeginChunk(AxMaterial.SerializationId_RenderState);
        writer.stream.WriteInt16(this.renderStates.Get(i));
        writer.EndChunk();
    }

    writer.EndChunk();

    // Make base serialization at end, because shader data creates properties, which need to be filled afterwards
    AxResource.prototype.SerializeChunks.call(this, writer);
};

/**
 * Reads the data of a chunk. The chunk header is already read by the reader and this method deserializes the contents of the chunk. Called continuously for each of the mesh's chunks
 * @param {AxHierarchyStreamReader} reader Reader to use for reading the serialized data.
 * @return {Boolean} True if a chunk was successfully deserialized
 */
AxMaterial.prototype.DeserializeChunk = function(reader)
{
    if (AxResource.prototype.DeserializeChunk.call(this, reader))
        return true;

    switch (reader.chunkId)
    {
        case AxMaterial.SerializationId_ShadingLayers:
        {
            break;
        }

        case AxMaterial.SerializationId_ShadingLayer:
        {
            var shadingOp = reader.stream.ReadInt16();
            var colorOp = reader.stream.ReadInt16();
            this.AddShadingLayer(shadingOp, colorOp);
            break;
        }

        case AxMaterial.SerializationId_RenderStates:
        {
            break;
        }

        case AxMaterial.SerializationId_RenderState:
        {
            this.renderStates.Add(reader.stream.ReadInt16());
            break;
        }

        default:
        {
            return false;
        }
    }

    return true;
};
