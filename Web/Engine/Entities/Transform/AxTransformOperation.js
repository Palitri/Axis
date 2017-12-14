/**
 * Represents a trasform operation
 * Transform operations are used by transform layers, which in turn are used in transforms to define a transforming operation
 */
var AxTransformOperation = Object.freeze(
{
    Translate : 0,
    RotateX : 1,
    RotateY : 2,
    RotateZ : 3,
    Scale : 4,
    TranslateAbsolute : 5,
    RotateAbsoluteX : 6,
    RotateAbsoluteY : 7,
    RotateAbsoluteZ : 8,
    ScaleAbsolute : 9,
    Reflect : 10,
    Skybox : 11,
    Billboard : 12
});