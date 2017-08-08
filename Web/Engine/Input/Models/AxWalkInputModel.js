/**
 * Creates an input model, providing a walk-like manipulation on a given transform
 * The walk model assumes a well defined "vertical" orientation and in contrast to the fly model, it always turns around this fixed vertical axis. This model assumes Y as the vertical axis.
 * For example, a person walking on the ground would always turn around the orientation of the gravity vector (which is generally perpendicular to the ground) no matter whether that person is facing the horizon or the sky or the ground.
 * In contrast, in a free flight model, where there are no fixed directions, an analogical turning would be around the viewer's own vertical axis, which would be oriented differently if the viewer is facing the horizon, the sky or the ground.
 * For multiple alternative inputs on the same control, enumerate all the input names, separated by comma.
 * Both digital and analog inputs are acceptable, such as keyboard, mouse, gamepad buttons as well as mouse coordinates and gamepad thumbsticks and triggers
 * For analog inputs, which provide positive and negative values, only one of two opposing controls is required. For example, when using the mouse to turn left and right, only the left or only the right control is sufficient in this case, because a negative value from the input would reverse the turning direction.
 * The model provides inputs to serve as conditions for moving and turning. If these inputs are provided, moving and turning respectively will only occur if the given conditional input is active. For example, to turn by moving the mouse, but only while mouse button is pressed, the mouse button must be provided as conditional input.
 * @param {Axis} context The instance of Axis, to which the model will be applied
 * @param {AxTransform} transform The transform for which to perform the manipulation of the input model
 * @param {Number} moveSpeed The speed multiplier in units per second per input at which to move
 * @param {Number} moveResponsiveness The moving responsiveness, where 0 is no responsiveness at all and greater values result in a more responsive movement. Smaller values result in a more inert moving. Must be greater than 0 in order for any movement to occur
 * @param {Number} turnSpeed The turning speed multiplier in turns per second per input at which to rotate
 * @param {Number} turnResponsiveness The turning responsiveness, where 0 is no responsiveness at all and greater values result in a more responsive turning. Smaller values result in a more inert turning. Must be greater than 0 in order for any movement to occur
 * @param {String|AxString} forwardInput Names of the inputs to control the forward movement. Passing a null or empty string value would discard the control.
 * @param {String|AxString} backwardInput Names of the inputs to control the backward movement. Passing a null or empty string value would discard the control.
 * @param {String|AxString} leftInput Names of the inputs to control the left movement. Passing a null or empty string value would discard the control.
 * @param {String|AxString} rightInput Names of the inputs to control the right movement. Passing a null or empty string value would discard the control.
 * @param {String|AxString} upInput Names of the inputs to control the up movement. Passing a null or empty string value would discard the control.
 * @param {String|AxString} downInput Names of the inputs to control the down movement. Passing a null or empty string value would discard the control.
 * @param {String|AxString} moveConditionInput Names of the inputs to serve as a condition for allowing movement. Passing a null or empty string value would discard the condition.
 * @param {String|AxString} turnUpInput Names of the inputs to control turning up. Passing a null or empty string value would discard the control.
 * @param {String|AxString} turnDownInput Names of the inputs to control turning down. Passing a null or empty string value would discard the control.
 * @param {String|AxString} turnLeftInput Names of the inputs to control turning to the left. Passing a null or empty string value would discard the control.
 * @param {String|AxString} turnRightInput Names of the inputs to control turning to the right. Passing a null or empty string value would discard the control.
 * @param {String|AxString} turnConditionInput Names of the inputs to serve as a condition for allowing turning. Passing a null or empty string value would discard the condition.
 * @constructor
 */
function AxWalkInputModel(context, transform, moveSpeed, moveResponsiveness, turnSpeed, turnResponsiveness, forwardInput, backwardInput, leftInput, rightInput, upInput, downInput, moveConditionInput, turnUpInput, turnDownInput, turnLeftInput, turnRightInput, turnConditionInput)
{
    AxInputModel.call(this);

    this.moveVector = new AxVector3();
    this.turnVector = new AxVector2();

    this.forwardInput = new AxInputControls();
    this.backwardInput = new AxInputControls();
    this.leftInput = new AxInputControls();
    this.rightInput = new AxInputControls();
    this.upInput = new AxInputControls();
    this.downInput = new AxInputControls();
    this.moveConditionInput = new AxInputControls();
    this.turnUpInput = new AxInputControls();
    this.turnDownInput = new AxInputControls();
    this.turnLeftInput = new AxInputControls();
    this.turnRightInput = new AxInputControls();
    this.turnConditionInput = new AxInputControls();
    
    this.context = context;
    this.transform = transform;

    this.context.input.GetInputControls(this.forwardInput, forwardInput);
    this.context.input.GetInputControls(this.backwardInput, backwardInput);
    this.context.input.GetInputControls(this.leftInput, leftInput);
    this.context.input.GetInputControls(this.rightInput, rightInput);
    this.context.input.GetInputControls(this.upInput, upInput);
    this.context.input.GetInputControls(this.downInput, downInput);
    this.context.input.GetInputControls(this.moveConditionInput, moveConditionInput);
    this.context.input.GetInputControls(this.turnUpInput, turnUpInput);
    this.context.input.GetInputControls(this.turnDownInput, turnDownInput);
    this.context.input.GetInputControls(this.turnLeftInput, turnLeftInput);
    this.context.input.GetInputControls(this.turnRightInput, turnRightInput);
    this.context.input.GetInputControls(this.turnConditionInput, turnConditionInput);

    this.moveSpeed = moveSpeed;
    this.moveResponsiveness = moveResponsiveness;
    this.turnSpeed = turnSpeed;
    this.turnResponsiveness = turnResponsiveness;
}

AxWalkInputModel.prototype = Object.create(AxInputModel.prototype);


/**
 * Performs the input model routines
 */
AxWalkInputModel.prototype.Process = function()
{
    if (AxUtils.IsUndefinedOrNull(this.transform))
        return;

    var speedFactor = this.context.timer.actualTime;

    var moveVector = new AxVector3();
    if ((this.moveConditionInput.count === 0) || this.moveConditionInput.GetValue(speedFactor) !== 0.0)
    {
        moveVector.x = this.leftInput.GetValue(speedFactor) - this.rightInput.GetValue(speedFactor);
        moveVector.y = this.upInput.GetValue(speedFactor) - this.downInput.GetValue(speedFactor);
        moveVector.z = this.forwardInput.GetValue(speedFactor) - this.backwardInput.GetValue(speedFactor);
    }
    else
        moveVector.Set(0.0);

    AxVector3.Lerp(this.moveVector, this.moveVector, moveVector, AxMath.Trim(this.moveResponsiveness * speedFactor, 0.0, 1.0));

    this.transform.TranslateIntrinsic(this.moveVector.Scale(this.moveSpeed));


    var turnVector = new AxVector2();
    if ((this.turnConditionInput.count === 0) || this.turnConditionInput.GetValue(speedFactor) !== 0.0)
    {
        turnVector.x = this.turnUpInput.GetValue(speedFactor) - this.turnDownInput.GetValue(speedFactor);
        turnVector.y = this.turnLeftInput.GetValue(speedFactor) - this.turnRightInput.GetValue(speedFactor);
    }
    else
        turnVector.Set(0.0);

    AxVector2.Lerp(this.turnVector, this.turnVector, turnVector, AxMath.Trim(this.turnResponsiveness * speedFactor, 0.0, 1.0));

    var turnSpeedFactor = this.turnSpeed * AxMath.Pi2;
    this.transform.RotateExtrinsic(new AxVector3(0.0, this.turnVector.y * turnSpeedFactor, 0.0));
    this.transform.RotateIntrinsic(new AxVector3(this.turnVector.x * turnSpeedFactor, 0.0, 0.0));
};
