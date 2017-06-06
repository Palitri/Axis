/**
 * This is a virtual prototype meant to be inherited by overrideing its methods
 * Creates a new entity dispatcher
 * Entity dispatchers are used to create on demand entities by a given type id. This allows abstract entity creation, especially useful in deserialization
 */
function AxEntityDispatcher()
{
    
}

/**
 * Creates an entity of the given typeId
 * @param {Axis} context Context to use for the creation of the entity
 * @param {Integer} typeId Identificator for the entity type to create
 * @return {AxEntity} The created entity object
 */
AxEntityDispatcher.prototype.CreateEntity = function(context, typeId) { };