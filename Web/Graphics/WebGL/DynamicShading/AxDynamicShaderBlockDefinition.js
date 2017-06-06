/**
 * Creates a new shader definition for building a dynamic shader
 * The shader definition is a name and code pair, in which the name is a key and a semanting meaning and the code is the source code of the definition
 * @constructor
 */
function AxDynamicShaderDefinition()
{
    this.name = null;
    this.code = null;
}



/**
 * Creates a new colleciton of shader definitions
 * The colleciton is, in fact, a dictionary of code blocks, where each code block is defined by a name, which acts as a key and represents the semanting meaning of the block
 * This way, when a block of code with a specific semantic purpose is required, 
 * @constructor
 */
function AxDynamicShaderBlockDefinitions()
{
    this.definitions = new AxList();
}



AxDynamicShaderBlockDefinitions.prototype.Dispose = function()
{
    for (var i = 0; i < this.definitions.count; i++)
        this.definitions.Set(i, null);
};

/**
 * Provides a code definition with a name, which acts as a key.
 * The name acts as a key for the definition and if there is already a definition with the same name, no new definition is added, the definitions collection remains the same and the return value is false is false.\
 * This method ensures that the code, which is to be defined is available and doesn't duplicate
 * @param {AxString} name The name of the code definition. It acts as a key and as a semantic meaning for the code, which is to be defined. If the name is undefined, null or empty, the definition is added unconditionally and isn't checked against for later definitions
 * @param {AxString} code The actual source code of the definition
 * @return {Boolean} True if a definition was successfully added. False otherwise, for example if a definition with the same name was already defined.
 */
AxDynamicShaderBlockDefinitions.prototype.ProvideDefinition = function(name, code)
{
    name = AxString.GetAxString(name);
    code = AxString.GetAxString(code);
    
    if (!AxString.IsNullOrEmpty(name))
        for (var i = 0; i < this.definitions.count; i++)
            if (this.definitions.Get(i).name.Equals(name))
                return false;

    var definition = new AxDynamicShaderDefinition();
    definition.name = name;
    definition.code = code;
    this.definitions.Add(definition);

    return true;
};

/**
 * Generates all the source code from the whole collection of definitions
 * @return {AxString} The full source code, compiled by concatenating all the definitions in the collection
 */
AxDynamicShaderBlockDefinitions.prototype.GetCode = function()
{
    var result = new AxString();

    for (var i = 0; i < this.definitions.count; i++)
    {
        result.Insert(this.definitions.Get(i).code);
        result.Insert(new AxString('\r\n'));
    }

    return result;
};