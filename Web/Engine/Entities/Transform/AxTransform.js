/**
 * Creates a new transform
 * @constructor
 */
function AxTransform()
{
    AxResource.call(this);

    this.typeId = AxTransform.classId;
    this.resourceType = AxResourceType.Transform;

    this.properties.Add(new AxProperty(new AxString('Bone'), false));
    this.properties.Add(new AxProperty(new AxString('Bone Index'), 0));
    
    this.oldPivot = new AxMatrix();
    this.pivotInverse = new AxMatrix();
    this.m = new AxMatrix();

    this.boneBindPoseInverse = new AxMatrix();
    this.pivot = new AxMatrix();
    this.transform = new AxMatrix();
    this.localMatrix = new AxMatrix();
    this.worldMatrix = new AxMatrix();
    this.pivotedWorldMatrix = new AxMatrix();
    
    AxMatrix.CreateIdentity(this.pivot);
    AxMatrix.CreateIdentity(this.transform);

    this.transformLayers = new AxList();
    
    this.parent = null;

    this._updateState = false;
}

AxTransform.prototype = Object.create(AxResource.prototype);

AxTransform.classId = (AxResourceType.Transform << 16) | 0;

AxTransform.propertyIndex_Bone      = AxResource.propertyIndex_ChildPropertiesIndex + 0;
AxTransform.propertyIndex_BoneIndex = AxResource.propertyIndex_ChildPropertiesIndex + 1;

AxTransform.SerializationId_Pivot		= 0x21111002;
AxTransform.SerializationId_Transform           = 0x21112002;
AxTransform.SerializationId_TransformLayers	= 0x21113002;
AxTransform.SerializationId_TransformLayer	= 0x21113102;
AxTransform.SerializationId_BindInverse		= 0x21114002;


AxTransform.prototype.Dispose = function()
{
};


/**
 * Inserts a transformation layer at a given index
 * @param {AxTransformOperation} operation The transformation operation for the new layer to insert
 * @param {Integer} index Position in the transorm's properties where the new layer should be inserted
 */
AxTransform.prototype.InsertTransformLayer = function(operation, index)
{
    var numProperties = this.properties.count;

    var propertyIndex;
    if (index < this.transformLayers.count)
        propertyIndex = this.transformLayers.Get(index).propertyFirst;
    else
        propertyIndex = numProperties;

    var propertyFirst = propertyIndex;

    switch (operation)
    {
        case AxTransformOperation.Translate:
            this.properties.Insert(propertyIndex++, new AxProperty(new AxString('Translate X'), 0.0));
            this.properties.Insert(propertyIndex++, new AxProperty(new AxString('Translate Y'), 0.0));
            this.properties.Insert(propertyIndex++, new AxProperty(new AxString('Translate Z'), 0.0));
            break;

        case AxTransformOperation.RotateX:
            this.properties.Insert(propertyIndex++, new AxProperty(new AxString('Rotate X'), 0.0));
            break;

        case AxTransformOperation.RotateY:
            this.properties.Insert(propertyIndex++, new AxProperty(new AxString('Rotate Y'), 0.0));
            break;

        case AxTransformOperation.RotateZ:
            this.properties.Insert(propertyIndex++, new AxProperty(new AxString('Rotate Z'), 0.0));
            break;

        case AxTransformOperation.Scale:
            this.properties.Insert(propertyIndex++, new AxProperty(new AxString('Scale X'), 1.0));
            this.properties.Insert(propertyIndex++, new AxProperty(new AxString('Scale Y'), 1.0));
            this.properties.Insert(propertyIndex++, new AxProperty(new AxString('Scale Z'), 1.0));
            break;

        case AxTransformOperation.TranslateAbsolute:
            this.properties.Insert(propertyIndex++, new AxProperty(new AxString('Translate AbsoluteX'), 0.0));
            this.properties.Insert(propertyIndex++, new AxProperty(new AxString('Translate AbsoluteY'), 0.0));
            this.properties.Insert(propertyIndex++, new AxProperty(new AxString('Translate AbsoluteZ'), 0.0));
            break;

        case AxTransformOperation.RotateAbsoluteX:
            this.properties.Insert(propertyIndex++, new AxProperty(new AxString('Rotate AbsoluteX'), 0.0));
            break;

        case AxTransformOperation.RotateAbsoluteY:
            this.properties.Insert(propertyIndex++, new AxProperty(new AxString('Rotate AbsoluteY'), 0.0));
            break;

        case AxTransformOperation.RotateAbsoluteZ:
            this.properties.Insert(propertyIndex++, new AxProperty(new AxString('Rotate AbsoluteZ'), 0.0));
            break;

        case AxTransformOperation.ScaleAbsolute:
            this.properties.Insert(propertyIndex++, new AxProperty(new AxString('Scale AbsoluteX'), 1.0));
            this.properties.Insert(propertyIndex++, new AxProperty(new AxString('Scale AbsoluteY'), 1.0));
            this.properties.Insert(propertyIndex++, new AxProperty(new AxString('Scale AbsoluteZ'), 1.0));
            break;
    }

    var numNewProperties = this.properties.count - numProperties;
    this.transformLayers.Insert(index, new AxTransformLayer(operation, propertyFirst, numNewProperties));

    var count = this.transformLayers.count;
    for (var i = index + 1; i < count; i++)
        this.transformLayers.Get(i).propertyFirst += numNewProperties;
};

/**
 * Inserts a transformation layer after all the current transformation layers
 * @param {AxTransformOperation} operation The transformation operation for the new layer to insert
 * @return {Integer} The index at which the new transformation layer was inserted
 */
AxTransform.prototype.AddTransformLayer = function(operation)
{
    this.InsertTransformLayer(operation, this.transformLayers.count);
    return this.transformLayers.count - 1;
};

/**
 * Removes a transformation layer at a given index
 * @param {Integer} index The index of the transformation layer which is to be removed
 */
AxTransform.prototype.RemoveTransformLayer = function(index)
{
    var count = this.transformLayers.count;
    for (var i = index + 1; i < count; i++)
        this.transformLayers.Get(i).propertyFirst -= this.transformLayers.Get(i).propertyCount;

    this.properties.RemoveRange(this.transformLayers.Get(index).propertyFirst, this.transformLayers.Get(index).propertyCount);
	this.transformLayers.RemoveAt(index);
};

/**
 * Removes all transformation layers
 */
AxTransform.prototype.ClearTransformLayers = function()
{
    this.transformLayers.Clear();
    this.properties.Clear();
};

/**
 * Performs all transformation operatoins
 * @param {AxTransform} parent The parent transform onto which to calculate the world matrices of this transform. Of omitted or null, no parent transformation is applied
 */
AxTransform.prototype.Process = function(parent)
{
    this.parent = parent;
    
    if (!this.oldPivot.Equals(this.pivot))
    {
        AxMatrix.Invert(this.pivotInverse, this.pivot);
        AxMatrix.Copy(this.oldPivot, this.pivot);
    }

    AxMatrix.Copy(this.localMatrix, this.transform);

    var cameraTransform = null;
    var transformsCount = this.transformLayers.count;
    if (transformsCount !== 0)
    {
        propertyIndex = this.transformLayers.Get(0).propertyFirst;
        for (var i = 0; i < transformsCount; i++)
	{
            switch (this.transformLayers.Get(i).operation)
            {
                case AxTransformOperation.Translate:
                {
                    AxMatrix.CreateTranslation(this.m,
                        this.properties.Get(propertyIndex).GetEffectiveParameter().value,
			this.properties.Get(propertyIndex + 1).GetEffectiveParameter().value,
			this.properties.Get(propertyIndex + 2).GetEffectiveParameter().value);
                    AxMatrix.Multiply(this.localMatrix, this.m, this.localMatrix);

                    propertyIndex += 3;
                    break;
                }

                case AxTransformOperation.RotateX:
                {
                    AxMatrix.CreateRotationX(this.m, this.properties.Get(propertyIndex).GetEffectiveParameter().value);
                    AxMatrix.Multiply(this.localMatrix, this.m, this.localMatrix);

                    propertyIndex++;
                    break;
                }

                case AxTransformOperation.RotateY:
                {
                    AxMatrix.CreateRotationY(this.m, this.properties.Get(propertyIndex).GetEffectiveParameter().value);
                    AxMatrix.Multiply(this.localMatrix, this.m, this.localMatrix);

                    propertyIndex++;
                    break;
                }

                case AxTransformOperation.RotateZ:
                {
                    AxMatrix.CreateRotationZ(this.m, this.properties.Get(propertyIndex).GetEffectiveParameter().value);
                    AxMatrix.Multiply(this.localMatrix, this.m, this.localMatrix);

                    propertyIndex++;
                    break;
                }

                case AxTransformOperation.Scale:
                {
                    AxMatrix.CreateScale(this.m,
                        this.properties.Get(propertyIndex).GetEffectiveParameter().value,
                        this.properties.Get(propertyIndex).GetEffectiveParameter().value,
                        this.properties.Get(propertyIndex).GetEffectiveParameter().value);
                    AxMatrix.Multiply(this.localMatrix, this.m, this.localMatrix);

                    propertyIndex += 3;
                    break;
                }

                case AxTransformOperation.TranslateAbsolute:
                {
                    AxMatrix.CreateTranslation(this.m,
                        this.properties.Get(propertyIndex).GetEffectiveParameter().value,
                        this.properties.Get(propertyIndex + 1).GetEffectiveParameter().value,
                        this.properties.Get(propertyIndex + 2).GetEffectiveParameter().value);
                    AxMatrix.Multiply(this.localMatrix, this.localMatrix, this.m);

                    propertyIndex += 3;
                    break;
                }

                case AxTransformOperation.RotateAbsoluteX:
                {
                    AxMatrix.CreateRotationX(this.m, this.properties.Get(propertyIndex).GetEffectiveParameter().value);
                    AxMatrix.Multiply(this.localMatrix, this.localMatrix, this.m);

                    propertyIndex++;
                    break;
                }

                case AxTransformOperation.RotateAbsoluteY:
                {
                    AxMatrix.CreateRotationY(this.m, this.properties.Get(propertyIndex).GetEffectiveParameter().value);
                    AxMatrix.Multiply(this.localMatrix, this.localMatrix, this.m);

                    propertyIndex++;
                    break;
                }

                case AxTransformOperation.RotateAbsoluteZ:
                {
                    AxMatrix.CreateRotationZ(this.m, this.properties.Get(propertyIndex).GetEffectiveParameter().value);
                    AxMatrix.Multiply(this.localMatrix, this.localMatrix, this.m);

                    propertyIndex++;
                    break;
                }

                case AxTransformOperation.ScaleAbsolute:
                {
                    AxMatrix.CreateScale(this.m,
                        this.properties.Get(propertyIndex).GetEffectiveParameter().value,
                        this.properties.Get(propertyIndex + 1).GetEffectiveParameter().value,
                        this.properties.Get(propertyIndex + 2).GetEffectiveParameter().value);
                    AxMatrix.Multiply(this.localMatrix, this.localMatrix, this.m);

                    propertyIndex += 3;
                    break;
                }
                    
                case AxTransformOperation.Reflect:
                {
                    var transform = this.properties.Get(propertyIndex).GetEffectiveValue();
                    if (transform !== null)
                    {
                        var planePoint = new AxVector3();
                        var planeNormal = new AxVector3();
                        AxMatrix.GetTranslation(planePoint, transform.worldMatrix);
                        AxMatrix.GetAxisY(planeNormal, transform.worldMatrix);
                        var plane = new AxVector4();
                        AxMaths.PlaneGeneralFromPointNormal(plane, planePoint, planeNormal);
                        AxMatrix.CreateReflectionPlane(this.localMatrix, plane);
                    }

                    propertyIndex += 1;

                    break;
                }

                case AxTransformOperation.Skybox:
                {
                    cameraTransform = this.properties.Get(propertyIndex).GetEffectiveValue();
                    propertyIndex += 1;

                    break;
                }        
            }
        }
    }

    if (AxUtils.IsUndefinedOrNull(this.parent))
        AxMatrix.Copy(this.worldMatrix, this.localMatrix);
    else 
        AxMatrix.Multiply(this.worldMatrix, this.localMatrix, this.parent.worldMatrix);

    AxMatrix.Multiply(this.pivotedWorldMatrix, this.pivotInverse, this.worldMatrix);
    
    if (cameraTransform !== null)
    {
        var location = new AxVector3();
        AxMatrix.GetTranslation(location, cameraTransform.worldMatrix);
        AxMatrix.SetTranslation(this.pivotedWorldMatrix, location);
        AxMatrix.SetScaling(this.pivotedWorldMatrix, new AxVector3(100.0));
    }    
};


/**
 * Returns a vector, oriented along the right direction of the transform in scene space
 * This is the vector along the local X axis of the transform in scene space
 * @returns {AxVector3} A vector oriented along the transform's right direction
 */
AxTransform.prototype.GetRightVectorExtrinsic = function()
{
    var result = new AxVector3();
    AxMatrix.GetAxisX(result, this.worldMatrix);
    return result;
};

/**
 * Returns a vector, oriented along the right direction of the transform in local space
 * This is the vector along the local X axis of the transform in local space
 * @returns {AxVector3} A vector oriented along the transform's right direction
 */
AxTransform.prototype.GetRightVectorIntrinsic = function()
{
    var result = new AxVector3();
    AxMatrix.GetAxisX(result, this.transform);
    return result;
};

/**
 * Returns a vector, oriented along the up direction of the transform in scene space
 * This is the vector along the local Y axis of the transform in scene space
 * @returns {AxVector3} A vector oriented along the transform's up direction
 */
AxTransform.prototype.GetUpVectorExtrinsic = function()
{
    var result = new AxVector3();
    AxMatrix.GetAxisY(result, this.worldMatrix);
    return result;
};

/**
 * Returns a vector, oriented along the up direction of the transform in local space
 * This is the vector along the local Y axis of the transform in local space
 * @returns {AxVector3} A vector oriented along the transform's up direction
 */
AxTransform.prototype.GetUpVectorIntrinsic = function()
{
    var result = new AxVector3();
    AxMatrix.GetAxisY(result, this.transform);
    return result;
};

/**
 * Returns a vector, oriented along the forward direction of the transform in scene space
 * This is the vector along the local Z axis of the transform in scene space
 * @returns {AxVector3} A vector oriented along the transform's forward direction
 */
AxTransform.prototype.GetForwardVectorExtrinsic = function()
{
    var result = new AxVector3();
    AxMatrix.GetAxisZ(result, this.worldMatrix);
    return result;
};

/**
 * Returns a vector, oriented along the forward direction of the transform in local space
 * This is the vector along the local Z axis of the transform in local space
 * @returns {AxVector3} A vector oriented along the transform's forward direction
 */
AxTransform.prototype.GetForwardVectorIntrinsic = function()
{
    var result = new AxVector3();
    AxMatrix.GetAxisZ(result, this.transform);
    return result;
};

/**
 * Returns the position of the transform in scene space
 * @returns {AxVector3} The position of the transform in scene space
 */
AxTransform.prototype.GetPositionExtrinsic = function()
{
    var result = new AxVector3();
    AxMatrix.GetTranslation(result, this.worldMatrix);
    return result;
};

/**
 * Returns the position of the transform in local space
 * @returns {AxVector3} The position of the transform in scene space
 */
AxTransform.prototype.GetPositionIntrinsic = function()
{
    var result = new AxVector3();
    AxMatrix.GetTranslation(result, this.localMatrix);
    return result;
};

/**
 * Sets the position of the transform in scene space
 * @param {AxVector3} position The position in scene space to apply on the transform
 */
AxTransform.prototype.SetPositionExtrinsic = function(position)
{
    if (AxUtils.IsUndefinedOrNull(this.parent))
    {
        AxMatrix.SetTranslation(this.transform, position);
    }
    else
    {
        var parentInvert = new AxMatrix();
        AxMatrix.Invert(parentInvert, this.parent.worldMatrix);
        AxMatrix.Multiply(this.transform, this.transform, this.parent.worldMatrix);

        AxMatrix.SetTranslation(this.transform, position);

        AxMatrix.Multiply(this.transform, this.transform, parentInvert);
    }

    this.Process(this.parent);
};

/**
 * Sets the position of the transform in local space
 * @param {AxVector3} position The position in local space to apply on the transform
 */
AxTransform.prototype.SetPositionIntrinsic = function(position)
{
    AxMatrix.SetTranslation(this.transform, position);

    this.Process(this.parent);
};

/**
 * Get the rotation angles of the transform in scene space
 * The angles are extracted for an Euler rotation of XYZ sequence
 * @returns {AxVector3} The rotation angles in scene space of the transform, for a rotation in XYZ sequence
 */
AxTransform.prototype.GetRotationExtrinsic = function()
{
    var result = new AxVector3();
    AxMatrix.ExtractEulerRotationXYZ(result, this.worldMatrix);
    return result;
};

/**
 * Get the rotation angles of the transform in local space
 * The angles are extracted for an Euler rotation of XYZ sequence
 * @returns {AxVector3} The rotation angles in local space of the transform, for a rotation in XYZ sequence
 */
AxTransform.prototype.GetRotationIntrinsic = function()
{
    var result = new AxVector3();
    AxMatrix.ExtractEulerRotationXYZ(result, this.transform);
    return result;
};

/**
 * Sets the rotation angles of the transform in scene space
 * The rotation is performed in XYZ sequence
 * @param {AxVector3} rotation A vector with the rotation angles to be applied on each corresponding axis
 */
AxTransform.prototype.SetRotationExtrinsic = function(rotation)
{
    var scaling = new AxVector3();
    var translation = new AxVector3();
    AxMatrix.GetScaling(scaling, this.transform);
    AxMatrix.GetTranslation(translation, this.transform);

    if (AxUtils.IsUndefinedOrNull(this.parent))
    {
        AxMatrix.CreateRotationX(this.transform, rotation.x);
        AxMatrix.CreateRotationY(this.m, rotation.y);
        AxMatrix.Multiply3x3(this.transform, this.transform, this.m);
        AxMatrix.CreateRotationZ(this.m, rotation.z);
        AxMatrix.Multiply3x3(this.transform, this.transform, this.m);
    }
    else
    {
        var parentInvert = new AxMatrix();
        AxMatrix.Invert(parentInvert, this.parent.worldMatrix);

        AxMatrix.CreateRotationX(this.transform, rotation.x);
        AxMatrix.CreateRotationY(this.m, rotation.y);
        AxMatrix.Multiply3x3(this.transform, this.transform, this.m);
        AxMatrix.CreateRotationZ(this.m, rotation.z);
        AxMatrix.Multiply3x3(this.transform, this.transform, this.m);

        AxMatrix.Multiply(this.transform, this.transform, parentInvert);
    }

    AxMatrix.SetTranslation(this.transform, translation);
    AxMatrix.SetScaling(this.transform, scaling);
    
    this.Process(this.parent);
};

/**
 * Sets the rotation angles of the transform in local space
 * The rotation is performed in XYZ sequence
 * @param {AxVector3} rotation A vector with the rotation angles to be applied on each corresponding axis
 */
AxTransform.prototype.SetRotationIntrinsic = function(rotation)
{
    var scaling = new AxVector3();
    var translation = new AxVector3();
    AxMatrix.GetScaling(scaling, this.transform);
    AxMatrix.GetTranslation(translation, this.transform);

    AxMatrix.CreateRotationX(this.transform, rotation.x);
    AxMatrix.CreateRotationY(this.m, rotation.y);
    AxMatrix.Multiply3x3(this.transform, this.transform, this.m);
    AxMatrix.CreateRotationZ(this.m, rotation.z);
    AxMatrix.Multiply3x3(this.transform, this.transform, this.m);

    AxMatrix.SetTranslation(this.transform, translation);
    AxMatrix.SetScaling(this.transform, scaling);

    this.Process(this.parent);
};

/**
 * Returns the transform's scaling along the scene axes
 * @returns {AxVector3} The scaling of the transform in scene space
 */
AxTransform.prototype.GetScalingExtrinsic = function()
{
    var result = new AxVector3();
    AxMatrix.GetScaling(result, this.worldMatrix);
    return result;
};

/**
 * Returns the transform's scaling along its local axes
 * @returns {AxVector3} The scaling of the transform in local space
 */
AxTransform.prototype.GetScalingIntrinsic = function()
{
    var result = new AxVector3();
    AxMatrix.GetScaling(result, this.transform);
    return result;
};

/**
 * Sets the scaling of the transform along the scene axes
 * @param {AxVector3} scaling A vector with the scaling values to be set on each corresponding axis
 */
AxTransform.prototype.SetScalingExtrinsic = function(scaling)
{
    if (AxUtils.IsUndefinedOrNull(this.parent))
    {
        AxMatrix.SetScaling(this.transform, scaling);
    }
    else
    {
        var parentInvert = new AxMatrix();
        AxMatrix.Invert(parentInvert, this.parent.worldMatrix);
        AxMatrix.Multiply(this.transform, this.transform, this.parent.worldMatrix);

        AxMatrix.SetScaling(this.transform, scaling);

        AxMatrix.Multiply(this.transform, this.transform, parentInvert);
    }

    this.Process(this.parent);
};

/**
 * Sets the scaling of the transform along its local axes
 * @param {AxVector3} scaling A vector with the scaling values to be set on each corresponding axis
 */
AxTransform.prototype.SetScalingIntrinsic = function(scaling)
{
    AxMatrix.SetScaling(this.transform, scaling);

    this.Process(this.parent);
};

/**
 * Translates along the given vector in scene space
 * The translation is performed along the scene's axes, where the change in each axis is denoted by the corresponding component of the given vector
 * @param {AxVector3} translation A vector in scene space along which to translate the transform
 */
AxTransform.prototype.TranslateExtrinsic = function(translation)
{
    AxMatrix.CreateTranslation(this.m, translation);
    AxMatrix.Multiply(this.transform, this.transform, this.m);

    this.Process(this.parent);
};

/**
 * Translates along the given vector in respect to the transform's local space
 * The translation is performed along the local orientation axes, where the change in each axis is denoted by the corresponding component of the given vector
 * Effectively, this is a translation along the orientation of the transform's own coordinate system. This is useful, for example, when translating along an object's own left-right, up-down, forward-backward direction
 * @param {AxVector3} translation A vector along which to translate in local space
 */
AxTransform.prototype.TranslateIntrinsic = function(translation)
{
    AxMatrix.CreateTranslation(this.m, translation);
    AxMatrix.Multiply(this.transform, this.m, this.transform);

    this.Process(this.parent);
};

/**
 * Rotates around the axes of the scene's coordinate system.
 * The angle by which to rotate around each axis is provided by the respective component of the given vector
 * The performed rotation is applied in a X-Y-Z sequence
 * @param {AxVector3} angles A vector with the values of the angles by which to rotate around each respective axis of the scene
 */
AxTransform.prototype.RotateExtrinsic = function(angles)
{
    if (AxUtils.IsUndefinedOrNull(this.parent))
    {
        AxMatrix.CreateRotationX(this.m, angles.x);
        AxMatrix.Multiply3x3(this.transform, this.transform, this.m);
        AxMatrix.CreateRotationY(this.m, angles.y);
        AxMatrix.Multiply3x3(this.transform, this.transform, this.m);
        AxMatrix.CreateRotationZ(this.m, angles.z);
        AxMatrix.Multiply3x3(this.transform, this.transform, this.m);
    }
    else
    {
        var parentInvert = new AxMatrix();
        AxMatrix.Invert(parentInvert, this.parent.worldMatrix);
        AxMatrix.Multiply(this.transform, this.transform, this.parent.worldMatrix);

        AxMatrix.CreateRotationX(this.m, angles.x);
        AxMatrix.Multiply3x3(this.transform, this.transform, this.m);
        AxMatrix.CreateRotationY(this.m, angles.y);
        AxMatrix.Multiply3x3(this.transform, this.transform, this.m);
        AxMatrix.CreateRotationZ(this.m, angles.z);
        AxMatrix.Multiply3x3(this.transform, this.transform, this.m);

        AxMatrix.Multiply(this.transform, this.transform, parentInvert);
    }

    this.Process(this.parent);
};

/**
 * Rotates around the axes of the transform's local coordinate system.
 * The angle by which to rotate around each axis is provided by the respective component of the given vector
 * The performed rotation is applied in a X-Y-Z sequence
 * @param {AxVector3} angles A vector with the values of the angles by which to rotate around each respective axis of the transform's local coordinate system
 */
AxTransform.prototype.RotateIntrinsic = function(angles)
{
    if (AxUtils.IsUndefinedOrNull(this.parent))
    {
        AxMatrix.CreateRotationAxis(this.m, this.GetRightVectorExtrinsic().Normalize(), angles.x);
        AxMatrix.Multiply3x3(this.transform, this.transform, this.m);
        AxMatrix.CreateRotationAxis(this.m, this.GetUpVectorExtrinsic().Normalize(), angles.y);
        AxMatrix.Multiply3x3(this.transform, this.transform, this.m);
        AxMatrix.CreateRotationAxis(this.m, this.GetForwardVectorExtrinsic().Normalize(), angles.z);
        AxMatrix.Multiply3x3(this.transform, this.transform, this.m);
    }
    else
    {
        var parentInvert = new AxMatrix();
        AxMatrix.Invert(parentInvert, this.parent.worldMatrix);
        AxMatrix.Multiply(this.transform, this.transform, this.parent.worldMatrix);

        AxMatrix.CreateRotationAxis(this.m, this.GetRightVectorExtrinsic().Normalize(), angles.x);
        AxMatrix.Multiply3x3(this.transform, this.transform, this.m);
        AxMatrix.CreateRotationAxis(this.m, this.GetUpVectorExtrinsic().Normalize(), angles.y);
        AxMatrix.Multiply3x3(this.transform, this.transform, this.m);
        AxMatrix.CreateRotationAxis(this.m, this.GetForwardVectorExtrinsic().Normalize(), angles.z);
        AxMatrix.Multiply3x3(this.transform, this.transform, this.m);

        AxMatrix.Multiply(this.transform, this.transform, parentInvert);
    }

    this.Process(this.parent);
};

/**
 * Rotates around an arbitraty axis in scene space
 * @param {AxVector3} axis An axis in scene space, around which to rotate
 * @param {Number} angle The angle by which to rotate
 */
AxTransform.prototype.RotateAroundExtrinsic = function(axis, angle)
{
    if (AxUtils.IsUndefinedOrNull(this.parent))
    {
        AxMatrix.CreateRotationAxis(this.m, axis, angle);
        AxMatrix.Multiply3x3(this.transform, this.transform, this.m);
    }
    else
    {
        var parentInvert = new AxMatrix();
        AxMatrix.Invert(parentInvert, this.parent.worldMatrix);
        AxMatrix.Multiply(this.transform, this.transform, this.parent.worldMatrix);

        AxMatrix.CreateRotationAxis(this.m, axis, angle);
        AxMatrix.Multiply3x3(this.transform, this.transform, this.m);

        AxMatrix.Multiply(this.transform, this.transform, parentInvert);
    }

    this.Process(this.parent);
};

/**
 * Rotates around an arbitraty axis in local space
 * @param {AxVector3} axis An axis in scene space, around which to rotate
 * @param {Number} angle The angle by which to rotate
 */
AxTransform.prototype.RotateAroundIntrinsic = function(axis, angle)
{
    var worldAxis = new AxVector3();
    AxVector3.TransformNormal(worldAxis, axis, this.worldMatrix);

    if (AxUtils.IsUndefinedOrNull(this.parent))
    {
        AxMatrix.CreateRotationAxis(this.m, worldAxis, angle);
        AxMatrix.Multiply3x3(this.transform, this.transform, this.m);
    }
    else
    {
        var parentInvert = new AxMatrix();
        AxMatrix.Invert(parentInvert, this.parent.worldMatrix);
        AxMatrix.Multiply(this.transform, this.transform, this.parent.worldMatrix);

        AxMatrix.CreateRotationAxis(this.m, worldAxis, angle);
        AxMatrix.Multiply3x3(this.transform, this.transform, this.m);

        AxMatrix.Multiply(this.transform, this.transform, parentInvert);
    }

    this.Process(this.parent);
};

/**
 * Rotates around an arbitraty ray in scene space
 * In contrast to the RotateAroundExtrinsic method, which rotates around the transform's pivot, this method rotates around a specific ray located in the scene
 * @param {AxVector3} rayPoint A point on the ray
 * @param {AxVector3} rayVector The orientation vector of the ray
 * @param {Number} angle The angle by which to rotate
 */
AxTransform.prototype.RotateAroundRayExtrinsic = function(rayPoint, rayVector, angle)
{
    var rotationPivotPoint = new AxVector3();
    AxMaths.PointToRayProjection(rotationPivotPoint, this.GetPositionExtrinsic(), rayPoint, rayVector);

    if (AxUtils.IsUndefinedOrNull(this.parent))
    {
        AxMatrix.CreateTranslation(this.m, rotationPivotPoint.Invert());
        AxMatrix.Multiply(this.transform, this.transform, this.m);
        AxMatrix.CreateRotationAxis(this.m, rayVector, angle);
        AxMatrix.Multiply(this.transform, this.transform, this.m);
        AxMatrix.CreateTranslation(this.m, rotationPivotPoint);
        AxMatrix.Multiply(this.transform, this.transform, this.m);
    }
    else
    {
        var parentInvert = new AxMatrix();
        AxMatrix.Invert(parentInvert, this.parent.worldMatrix);
        AxMatrix.Multiply(this.transform, this.transform, this.parent.worldMatrix);

        AxMatrix.CreateTranslation(this.m, rotationPivotPoint.Invert());
        AxMatrix.Multiply(this.transform, this.transform, this.m);
        AxMatrix.CreateRotationAxis(this.m, rayVector, angle);
        AxMatrix.Multiply(this.transform, this.transform, this.m);
        AxMatrix.CreateTranslation(this.m, rotationPivotPoint);
        AxMatrix.Multiply(this.transform, this.transform, this.m);

        AxMatrix.Multiply(this.transform, this.transform, parentInvert);
    }

    this.Process(this.parent);
};

/**
 * Rotates around an arbitraty ray in local space
 * In contrast to the RotateAroundIntrinsic method, which rotates around the transform's pivot, this method rotates around a specific ray located in the transform's local space
 * @param {AxVector3} rayPoint A point on the ray
 * @param {AxVector3} rayVector The orientation vector of the ray
 * @param {Number} angle The angle by which to rotate
 */
AxTransform.prototype.RotateAroundRayIntrinsic = function(rayPoint, rayVector, angle)
{
    var rotationPivotPoint = new AxVector3();
    AxMaths.PointToRayProjection(rotationPivotPoint, this.GetPositionIntrinsic(), rayPoint, rayVector);

    if (AxUtils.IsUndefinedOrNull(this.parent))
    {
        AxMatrix.CreateTranslation(this.m, rotationPivotPoint.Invert());
        AxMatrix.Multiply(this.transform, this.transform, this.m);
        AxMatrix.CreateRotationAxis(this.m, rayVector, angle);
        AxMatrix.Multiply(this.transform, this.transform, this.m);
        AxMatrix.CreateTranslation(this.m, rotationPivotPoint);
        AxMatrix.Multiply(this.transform, this.transform, this.m);
    }
    else
    {
        var parentInvert = new AxMatrix();
        AxMatrix.Invert(parentInvert, this.parent.worldMatrix);
        AxMatrix.Multiply(this.transform, this.transform, this.parent.worldMatrix);

        AxMatrix.CreateTranslation(this.m, rotationPivotPoint.Invert());
        AxMatrix.Multiply(this.transform, this.transform, this.m);
        AxMatrix.CreateRotationAxis(this.m, rayVector, angle);
        AxMatrix.Multiply(this.transform, this.transform, this.m);
        AxMatrix.CreateTranslation(this.m, rotationPivotPoint);
        AxMatrix.Multiply(this.transform, this.transform, this.m);

        AxMatrix.Multiply(this.transform, this.transform, parentInvert);
    }

    this.Process(this.parent);
};

/**
 * Scales along the axes of the scene's coordinate system
 * @param {AxVector3} scaling A vector with the values by which to scale along each corresponding axis
 */
AxTransform.prototype.ScaleExtrinsic = function(scaling)
{
    if (AxUtils.IsUndefinedOrNull(this.parent))
    {
        AxMatrix.CreateScaling(this.m, scaling);
        AxMatrix.Multiply3x3(this.transform, this.transform, this.m);
    }
    else
    {
        var parentInvert = new AxMatrix();
        AxMatrix.Invert(parentInvert, this.parent.worldMatrix);
        AxMatrix.Multiply(this.transform, this.transform, this.parent.worldMatrix);

        AxMatrix.CreateScaling(this.m, scaling);
        AxMatrix.Multiply3x3(this.transform, this.transform, this.m);

        AxMatrix.Multiply(this.transform, this.transform, parentInvert);
    }

    this.Process(this.parent);
};

/**
 * Scales along the axes of the transform's local coordinate system
 * @param {AxVector3} scaling A vector with the values by which to scale along each corresponding axis
 */
AxTransform.prototype.ScaleIntrinsic = function(scaling)
{
    AxMatrix.CreateScaling(this.m, scaling);
    AxMatrix.Multiply3x3(this.transform, this.m, this.transform);

    this.Process(this.parent);
};

/**
 * Modifies the transform, so that in effect, it will remain situated the same in the scene, when related to identity, that is, when the parent of the transform is a transform with no operations applied
 * Useful when detaching the transform from a parent
 */
AxTransform.prototype.RelateToIdentity = function()
{
    AxMatrix.Copy(this.transform, this.worldMatrix);
};

/**
 * Modifies the transform, so that in effect, it will remain situated the same in the scene, when related the given transform, that is, when the given transform is its parent 
 * Useful when attaching the transform to another one
 * @param {!AxMatrix|AxTransform} parent A matrix or transform towards which this one's scene situation should be related to. If null or left undefined, the transform is related to idendity
 */
AxTransform.prototype.RelateTo = function(parent)
{
    if (AxUtils.IsUndefinedOrNull(parent))
    {
        this.RelateToIdentity();
    }
    else
    {
        var newParentInvert = new AxMatrix();
        if (!AxUtils.IsUndefinedOrNull(parent.worldMatrix))
            AxMatrix.Invert(newParentInvert, parent.worldMatrix);
        else
            AxMatrix.Invert(newParentInvert, parent);
        AxMatrix.Multiply(this.transform, this.worldMatrix, newParentInvert);
    }
};

/**
 * Writes chunks for all the data which is needed to serialize the transform.
 * @param {AxHierarchyStreamWriter} writer Writer to use for writing the serialization data
 */
AxTransform.prototype.SerializeChunks = function(writer)
{
    writer.BeginChunk(AxTransform.SerializationId_Pivot);
    AxSerializationUtils.SerializeMatrix(writer.stream, this.pivot);
    writer.EndChunk();

    writer.BeginChunk(AxTransform.SerializationId_Transform);
    AxSerializationUtils.SerializeMatrix(writer.stream, this.transform);
    writer.EndChunk();

    writer.BeginChunk(AxTransform.SerializationId_BindInverse);
    AxSerializationUtils.SerializeMatrix(writer.stream, this.boneBindPoseInverse);
    writer.EndChunk();

    writer.BeginChunk(AxTransform.SerializationId_TransformLayers);
    for (var i = 0; i < this.transformLayers.count; i++)
    {
        writer.BeginChunk(AxTransform.SerializationId_TransformLayer);
        writer.stream.WriteInt16(this.transformLayers.Get(i).operation);
        writer.EndChunk();
    }
    writer.EndChunk();

    // Base serialization. Call it at the end, because transformation layers create properties, which need to be filled afterwards
    AxResource.prototype.SerializeChunks.call(this, writer);
};

/**
 * Reads the data of a chunk. The chunk header is already read by the reader and this method deserializes the contents of the chunk. Called continuously for each of the transform's chunks
 * @param {AxHierarchyStreamReader} reader Reader to use for reading the serialized data.
 * @return {Boolean} True if a chunk was successfully deserialized
 */
AxTransform.prototype.DeserializeChunk = function(reader)
{
    if (AxResource.prototype.DeserializeChunk.call(this, reader))
        return true;

    switch (reader.chunkId)
    {
        case AxTransform.SerializationId_Pivot:
        {
            AxSerializationUtils.DeserializeMatrix(this.pivot, reader.stream);
            break;
        }

        case AxTransform.SerializationId_Transform:
        {
            AxSerializationUtils.DeserializeMatrix(this.transform, reader.stream);
            break;
        }

        case AxTransform.SerializationId_BindInverse:
        {
            AxSerializationUtils.DeserializeMatrix(this.boneBindPoseInverse, reader.stream);
            break;
        }

        case AxTransform.SerializationId_TransformLayers:
        {
            break;
        }

        case AxTransform.SerializationId_TransformLayer:
        {
            this.AddTransformLayer(reader.stream.ReadInt16());
            break;
        }

        default:
        {
            return false;
        }
    }

    return true;
};