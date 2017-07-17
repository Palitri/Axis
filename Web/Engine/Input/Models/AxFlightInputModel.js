/**
 * Creates an input model, providing a free flight-like manipulation on a given transform
 * The flight model assumes no defined orientations and in contrast to the walk model, it turns around its local axes.
 * For example, an object in a free flight, who is turning left and right does so around its own local vertical axis, which changes its orientation with the viewier's rotation in space and would be oriented differently if the viewer is facing the horizon, the sky or the ground.
 * In contrast, a person walking on the ground would always turn around the orientation of the gravity vector (which is generally perpendicular to the ground) no matter whether that person is facing the horizon or the sky or the ground.
 * For multiple alternative inputs on the same control, enumerate all the input names, separated by comma.
 * Both digital and analog inputs are acceptable, such as keyboard, mouse, gamepad buttons as well as mouse coordinates and gamepad thumbsticks and triggers
 * For analog inputs, which provide positive and negative values, only one of two opposing controls is required. For example, when using the mouse to turn left and right, only the left or only the right control is sufficient in this case, because a negative value from the input would reverse the turning direction.
 * The model provides inputs to serve as conditions for moving and turning. If these inputs are provided, moving and turning respectively will only occur if the given conditional input is active. For example, to turn by moving the mouse, but only while mouse button is pressed, the mouse button must be provided as conditional input.
 * @param {Axis} context The instance of Axis, to which the model will be applied
 * @param {AxTransform} transform The transform for which to perform the manipulation of the input model
 * @param {Number} moveSpeed The transform for which to perform the manipulation of the input model
 * @param {Number} moveResponsiveness The transform for which to perform the manipulation of the input model
 * @param {Number} turnSpeed The transform for which to perform the manipulation of the input model
 * @param {Number} turnResponsiveness The transform for which to perform the manipulation of the input model
 * @param {String|AxString} forwardInput Names of the inputs to control the forward movement. Passing a null or empty string value would discard the control.
 * @param {String|AxString} backwardInput Names of the inputs to control the backward movement. Passing a null or empty string value would discard the control.
 * @param {String|AxString} leftInput Names of the inputs to control the left movement. Passing a null or empty string value would discard the control.
 * @param {String|AxString} rightInput Names of the inputs to control the right movement. Passing a null or empty string value would discard the control.
 * @param {String|AxString} upInput Names of the inputs to control the up movement. Passing a null or empty string value would discard the control.
 * @param {String|AxString} downInput Names of the inputs to control the down movement. Passing a null or empty string value would discard the control.
 * @param {String|AxString} moveConditionInput Names of the inputs to serve as a condition for allowing movement. Passing a null or empty string value would discard the condition.
 * @param {String|AxString} pitchUpInput Names of the inputs to control pitch up. Passing a null or empty string value would discard the control.
 * @param {String|AxString} pitchDownInput Names of the inputs to control pitch down. Passing a null or empty string value would discard the control.
 * @param {String|AxString} yawLeftInput Names of the inputs to control yaw to the left. Passing a null or empty string value would discard the control.
 * @param {String|AxString} yawRightInput Names of the inputs to control yaw to the right. Passing a null or empty string value would discard the control.
 * @param {String|AxString} rollLeftInput Names of the inputs to control roll to the left. Passing a null or empty string value would discard the control.
 * @param {String|AxString} rollRightInput Names of the inputs to control roll to the right. Passing a null or empty string value would discard the control.
 * @param {String|AxString} turnConditionInput Names of the inputs to serve as a condition for allowing turning. Passing a null or empty string value would discard the condition.
 * @constructor
 */
function AxFlightInputModel(context, transform, moveSpeed, moveResponsiveness, turnSpeed, turnResponsiveness, forwardInput, backwardInput, leftInput, rightInput, upInput, downInput, moveConditionInput, pitchUpInput, pitchDownInput, yawLeftInput, yawRightInput, rollLeftInput, rollRightInput, turnConditionInput)
{
    AxInputModel.call(this);

    this.moveVector = new AxVector3();
    this.turnVector = new AxVector3();

    this.forwardInput = new AxList();
    this.backwardInput = new AxList();
    this.leftInput = new AxList();
    this.rightInput = new AxList();
    this.upInput = new AxList();
    this.downInput = new AxList();
    this.moveConditionInput = new AxList();
    this.pitchUpInput = new AxList();
    this.pitchDownInput = new AxList();
    this.yawLeftInput = new AxList();
    this.yawRightInput = new AxList();
    this.rollLeftInput = new AxList();
    this.rollRightInput = new AxList();
    this.turnConditionInput = new AxList();

    this.context = context;
    this.transform = transform;

    AxInputModel.ParseInputNames(this.context.input, this.forwardInput, forwardInput);
    AxInputModel.ParseInputNames(this.context.input, this.backwardInput, backwardInput);
    AxInputModel.ParseInputNames(this.context.input, this.leftInput, leftInput);
    AxInputModel.ParseInputNames(this.context.input, this.rightInput, rightInput);
    AxInputModel.ParseInputNames(this.context.input, this.upInput, upInput);
    AxInputModel.ParseInputNames(this.context.input, this.downInput, downInput);
    AxInputModel.ParseInputNames(this.context.input, this.moveConditionInput, moveConditionInput);
    AxInputModel.ParseInputNames(this.context.input, this.pitchUpInput, pitchUpInput);
    AxInputModel.ParseInputNames(this.context.input, this.pitchDownInput, pitchDownInput);
    AxInputModel.ParseInputNames(this.context.input, this.yawLeftInput, yawLeftInput);
    AxInputModel.ParseInputNames(this.context.input, this.yawRightInput, yawRightInput);
    AxInputModel.ParseInputNames(this.context.input, this.rollLeftInput, rollLeftInput);
    AxInputModel.ParseInputNames(this.context.input, this.rollRightInput, rollRightInput);
    AxInputModel.ParseInputNames(this.context.input, this.turnConditionInput, turnConditionInput);

    this.moveSpeed = moveSpeed;
    this.moveResponsiveness = moveResponsiveness;
    this.turnSpeed = turnSpeed;
    this.turnResponsiveness = turnResponsiveness;
}

AxFlightInputModel.prototype = Object.create(AxInputModel.prototype);


/**
 * Performs the input model routines
 */
AxFlightInputModel.prototype.Process = function()
{
    if (AxUtils.IsUndefinedOrNull(this.transform))
        return;

    var speedFactor = this.context.timer.actualTime;

    var moveVector = new AxVector3();
    if ((this.moveConditionInput.count === 0) || AxInputModel.ProcessInputProperties(this.moveConditionInput, speedFactor) !== 0.0)
    {
        moveVector.x = AxInputModel.ProcessInputProperties(this.leftInput, speedFactor) - AxInputModel.ProcessInputProperties(this.rightInput, speedFactor);
        moveVector.y = AxInputModel.ProcessInputProperties(this.upInput, speedFactor) - AxInputModel.ProcessInputProperties(this.downInput, speedFactor);
        moveVector.z = AxInputModel.ProcessInputProperties(this.forwardInput, speedFactor) - AxInputModel.ProcessInputProperties(this.backwardInput, speedFactor);
    }
    else
        moveVector.Set(0.0);

    AxVector3.Lerp(this.moveVector, this.moveVector, moveVector, AxMath.Trim(this.moveResponsiveness * speedFactor, 0.0, 1.0));

    this.transform.TranslateIntrinsic(this.moveVector.Scale(this.moveSpeed));


    var turnVector = new AxVector3();
    if ((this.turnConditionInput.count === 0) || AxInputModel.ProcessInputProperties(this.turnConditionInput, speedFactor) !== 0.0)
    {
        turnVector.x = AxInputModel.ProcessInputProperties(this.pitchUpInput, speedFactor) - AxInputModel.ProcessInputProperties(this.pitchDownInput, speedFactor);
        turnVector.y = AxInputModel.ProcessInputProperties(this.yawLeftInput, speedFactor) - AxInputModel.ProcessInputProperties(this.yawRightInput, speedFactor);
        turnVector.z = AxInputModel.ProcessInputProperties(this.rollLeftInput, speedFactor) - AxInputModel.ProcessInputProperties(this.rollRightInput, speedFactor);
    }
    else
        turnVector.Set(0.0);

    AxVector3.Lerp(this.turnVector, this.turnVector, turnVector, AxMath.Trim(this.turnResponsiveness * speedFactor, 0.0, 1.0));

    var turnSpeedFactor = this.turnSpeed * AxMath.Pi2;
    this.transform.RotateIntrinsic(new AxVector3(this.turnVector.x * turnSpeedFactor, 0.0, 0.0));
    this.transform.RotateIntrinsic(new AxVector3(0.0, -this.turnVector.y * turnSpeedFactor, 0.0));
    this.transform.RotateIntrinsic(new AxVector3(0.0, 0.0, this.turnVector.z * turnSpeedFactor));
};