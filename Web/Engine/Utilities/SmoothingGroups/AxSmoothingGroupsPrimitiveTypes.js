/**
 * Holds all the indices that derive from a given original vertex and all their smooth groups accumulated
 * @constructor
 */
function AxSmoothingGroupsVertexInfo()
{
    this.accumulatedSmoothingGroups = 0;
    this.indices = new AxList();
}

/**
 * Holds the smoothing groups of a face and the indices of its vertices
 * @constructor
 */
function AxSmoothingGroupsFace()
{
    this.smoothingGroups = 0;
    this.indices = AxList();
}

/**
 * Holds the smooth groups of a vetex
 * @constructor
 */
function AxSmoothingGroupsVertex()
{
    this.accumulatedSmoothingGroups = 0;
    this.index = 0;
}
