/**
 * Creates an input model, which provides rotation manipulation of a transform
 * Useful for examinig and object by rotating it around an seeing it from different angles
 * For multiple alternative inputs on the same control, enumerate all the input names, separated by comma.
 * Both digital and analog inputs are acceptable, such as keyboard, mouse, gamepad buttons as well as mouse coordinates and gamepad thumbsticks and triggers
 * For analog inputs, which provide positive and negative values, only one of two opposing controls is required. For example, when using the mouse to turn left and right, only the left or only the right control is sufficient in this case, because a negative value from the input would reverse the turning direction.
 * The model provides inputs to serve as manipulation conditions. If these inputs are provided, manipulating will only occur if the given conditional input is active. For example, to turn by moving the mouse, but only while mouse button is pressed, the mouse button must be provided as conditional input.
 * @param {Axis} context The instance of Axis, to which the model will be applied
 * @param {AxTransform} transform The transform for which to perform the manipulation of the input model
 * @param {Number} rotationSpeed The rotation speed multiplier in turns per second per input at which to turn
 * @param {Number} rotationResponsiveness The rotation responsiveness, where 0 is no responsiveness at all and greater values result in a more responsive rotation. Smaller values result in a more inert rotation. Must be greater than 0 in order for any rotation to occur
 * @param {String|AxString} upInput Names of the inputs to control the up rotation. Passing a null or empty string value would discard the control.
 * @param {String|AxString} downInput Names of the inputs to control the down rotation. Passing a null or empty string value would discard the control.
 * @param {String|AxString} leftInput Names of the inputs to control the left rotation. Passing a null or empty string value would discard the control.
 * @param {String|AxString} rightInput Names of the inputs to control the right rotation. Passing a null or empty string value would discard the control.
 * @param {String|AxString} rollLeftInput Names of the inputs to control the roll left rotation. Passing a null or empty string value would discard the control.
 * @param {String|AxString} rollRightInput Names of the inputs to control the roll right rotation. Passing a null or empty string value would discard the control.
 * @param {String|AxString} rotationConditionInput Names of the inputs to serve as a condition for allowing rotation. Passing a null or empty string value would discard the condition.
 * @constructor
 */
function AxRotationInputModel(context, transform, rotationSpeed, rotationResponsiveness, upInput, downInput, leftInput, rightInput, rollLeftInput, rollRightInput, rotationConditionInput)
{
    AxInputModel.call(this);

    this.rotationVector = new AxVector3();

    this.upInput = new AxInputControls();
    this.downInput = new AxInputControls();
    this.leftInput = new AxInputControls();
    this.rightInput = new AxInputControls();
    this.rollLeftInput = new AxInputControls();
    this.rollRightInput = new AxInputControls();
    this.rotationConditionInput = new AxInputControls();

    this.context = context;
    this.transform = transform;

    this.rotationSpeed = rotationSpeed;
    this.rotationResponsiveness = rotationResponsiveness;

    this.context.input.GetInputControls(this.upInput, upInput);
    this.context.input.GetInputControls(this.downInput, downInput);
    this.context.input.GetInputControls(this.leftInput, leftInput);
    this.context.input.GetInputControls(this.rightInput, rightInput);
    this.context.input.GetInputControls(this.rollLeftInput, rollLeftInput);
    this.context.input.GetInputControls(this.rollRightInput, rollRightInput);
    this.context.input.GetInputControls(this.rotationConditionInput, rotationConditionInput);
}

AxRotationInputModel.prototype = Object.create(AxInputModel.prototype);


/**
 * Performs the input model routines
 */
AxRotationInputModel.prototype.Process = function(){
    if (AxUtils.IsUndefinedOrNull(this.transform))
        return;

    var speedFactor = this.context.timer.actualTime;

    var rotationVector = new AxVector3();
    if ((this.rotationConditionInput.count === 0) || this.rotationConditionInput.GetValue(speedFactor) !== 0.0)
    {
        rotationVector.x = this.leftInput.GetValue(speedFactor) - this.rightInput.GetValue(speedFactor);
        rotationVector.y = this.upInput.GetValue(speedFactor) - this.downInput.GetValue(speedFactor);
        rotationVector.z = this.rollLeftInput.GetValue(speedFactor) - this.rollRightInput.GetValue(speedFactor);
    }
    else
        rotationVector.Set(0.0);

    AxVector3.Lerp(this.rotationVector, this.rotationVector, rotationVector, AxMath.Trim(this.rotationResponsiveness * speedFactor, 0.0, 1.0));

    var rotationSpeedFactor = this.rotationSpeed * AxMath.Pi2;
    if (AxUtils.IsUndefinedOrNull(this.context.currentCamera.transformRef))
    {
        this.transform.RotateIntrinsic(new AxVector3(0.0, this.rotationVector.x * rotationSpeedFactor, 0.0));
        this.transform.RotateExtrinsic(new AxVector3(this.rotationVector.y * rotationSpeedFactor, 0.0, 0.0));
        this.transform.RotateExtrinsic(new AxVector3(0.0, 0.0, this.rotationVector.z * rotationSpeedFactor));
    }
    else
    {
        this.transform.RotateIntrinsic(new AxVector3(0.0, this.rotationVector.x * rotationSpeedFactor, 0.0));
        this.transform.RotateAroundExtrinsic(this.context.currentCamera.transformRef.GetRightVectorExtrinsic(), -this.rotationVector.y * rotationSpeedFactor);
        this.transform.RotateAroundExtrinsic(this.context.currentCamera.transformRef.GetForwardVectorExtrinsic(), this.rotationVector.z * rotationSpeedFactor);
    }
};
