/**
 * Creates an input model, which updates a property value according to a given input.
 * The value of the property is controlled by the given factor parameters and is calculated as follows:
 * originalValue * originalFactor + inputValue * inputFactor + originalValue * inputValue * multiplicationFactor
 * The property can be either Float, Int or Bool, where any absolute value equal or greater than 1.0 is considered true
 * @param {Axis} context The instance of Axis, to which the model will be applied
 * @param {AxProperty} linkedProperty The transform which is to act as a pivot for orbiting around
 * @param {String|AxString} valueInput Names of the inputs to use as input value. Passing a null or empty string value would discard the control.
 * @param {String|AxString} conditionInput Names of the inputs to serve as a condition to allow the linking. Passing a null or empty string value would discard the control.
 * @param {!Number} originalFactor The factor by which to multiply the original property value. If omitted, a value of 1.0 is assumed
 * @param {!Number} inputFactor The factor by which to multiply the input value. If omitted, a value of 1.0 is assumed
 * @param {!Number} multiplicationFactor The factor by which to multiply the product of the originalproperty value and the input value. If omitted, a value of 0.0 is assumed
 * @constructor
 */
function AxPropertyLinkInputModel(context, linkedProperty, valueInput, conditionInput, originalFactor, inputFactor, multiplicationFactor)
{
    AxInputModel.call(this);
    
    if (AxUtils.IsUndefinedOrNull(originalFactor))
        originalFactor = 1.0;
    if (AxUtils.IsUndefinedOrNull(inputFactor))
        inputFactor = 1.0;
    if (AxUtils.IsUndefinedOrNull(multiplicationFactor))
        multiplicationFactor = 0.0;

    this.linkedProperty = linkedProperty;
    this.originalFactor = originalFactor;
    this.inputFactor = inputFactor;
    this.multiplicationFactor = multiplicationFactor;
    
    this.valueInput = new AxInputControls();
    this.conditionInput = new AxInputControls();

    this.context = context;

    this.context.input.GetInputControls(this.valueInput, valueInput);
    this.context.input.GetInputControls(this.conditionInput, conditionInput);
}

AxPropertyLinkInputModel.prototype = Object.create(AxInputModel.prototype);


/**
 * Performs the input model routines
 */
AxPropertyLinkInputModel.prototype.Process = function()
{
    var originalValue;
    if (this.linkedProperty.type === AxParameterType.Float)
        originalValue = this.linkedProperty.GetFloat();
    else if (this.linkedProperty.type === AxParameterType.Int)
        originalValue = this.linkedProperty.GetInt();
    else if (this.linkedProperty.type === AxParameterType.Bool)
        originalValue = this.linkedProperty.GetBool() ? 1.0 : 0.0;
    else return;
        
    if ((this.conditionInput.count === 0) || this.conditionInput.GetValue(speedFactor) === 0.0)
        return;
    
    var speedFactor = this.context.timer.actualTime;
    var inputValue = ((this.conditionInput.count === 0) || this.conditionInput.GetValue(speedFactor) !== 0.0) ? this.valueInput.GetValue(speedFactor) : 0.0;
    var result = originalValue * this.originalFactor + inputValue * this.inputFactor + originalValue * inputValue * this.multiplicationFactor;
    
    
    if (this.linkedProperty.type === AxParameterType.Float)
        this.linkedProperty.SetFloat(result);
    else if (this.linkedProperty.type === AxParameterType.Int)
        this.linkedProperty.SetInt(AxMath.Trunc(result));
    else if (this.linkedProperty.type === AxParameterType.Bool)
        this.linkedProperty.SetBool(AxMath.Abs(result) >= 1.0);

};
