/**
 * @file bgi_dds.cpp
 * @brief DdsScene container implementation.
 */

#include "bgi_dds.h"
#include "bgi_state.h"

#include <algorithm>

namespace bgi
{

DdsScene::DdsScene()
{
    layers.emplace("DefaultLayer", DdsLayer{"DefaultLayer", true});
}

// ---------------------------------------------------------------------------
// DdsScene::appendWithId
// ---------------------------------------------------------------------------

std::shared_ptr<DdsObject> DdsScene::appendWithId(std::shared_ptr<DdsObject> obj)
{
    // Caller has already set obj->id; just insert.
    const auto type = obj->type;
    if (type == DdsObjectType::Camera ||
        type == DdsObjectType::Ucs ||
        type == DdsObjectType::WorldExtentsObj ||
        !layerExists(obj->layerName))
        obj->layerName = "DefaultLayer";
    order.push_back(obj->id);
    index[obj->id] = obj;
    ++revision;
    // Keep nextId ahead of any restored numeric id.
    try
    {
        uint64_t numId = std::stoull(obj->id);
        if (numId >= nextId)
            nextId = numId + 1;
    }
    catch (...) {}
    return obj;
}

// ---------------------------------------------------------------------------
// DdsScene::append
// ---------------------------------------------------------------------------

std::shared_ptr<DdsObject> DdsScene::append(std::shared_ptr<DdsObject> obj)
{
    obj->id = genId();
    const auto type = obj->type;
    if (type == DdsObjectType::Camera ||
        type == DdsObjectType::Ucs ||
        type == DdsObjectType::WorldExtentsObj ||
        !layerExists(obj->layerName))
        obj->layerName = "DefaultLayer";
    order.push_back(obj->id);
    index[obj->id] = obj;
    ++revision;
    return obj;
}

bool DdsScene::createLayer(const std::string &name)
{
    if (name.empty() || layers.find(name) != layers.end())
        return false;
    layers.emplace(name, DdsLayer{name, true});
    ++revision;
    return true;
}

void DdsScene::resetLayers()
{
    layers.clear();
    layers.emplace("DefaultLayer", DdsLayer{"DefaultLayer", true});
    ++revision;
}

bool DdsScene::deleteLayer(const std::string &name)
{
    if (name.empty() || name == "DefaultLayer" || layers.find(name) == layers.end())
        return false;
    clearLayer(name);
    layers.erase(name);
    ++revision;
    return true;
}

bool DdsScene::clearLayer(const std::string &name)
{
    if (layers.find(name) == layers.end())
        return false;
    for (const auto &id : order)
    {
        auto it = index.find(id);
        if (it == index.end() || it->second->deleted || it->second->layerName != name)
            continue;
        const auto type = it->second->type;
        if (type != DdsObjectType::Camera &&
            type != DdsObjectType::Ucs &&
            type != DdsObjectType::WorldExtentsObj)
            it->second->deleted = true;
    }
    compact();
    ++revision;
    return true;
}

bool DdsScene::setLayerVisible(const std::string &name, bool visible)
{
    auto it = layers.find(name);
    if (it == layers.end())
        return false;
    if (it->second.visible != visible)
    {
        it->second.visible = visible;
        ++revision;
    }
    return true;
}

bool DdsScene::layerExists(const std::string &name) const
{
    return layers.find(name) != layers.end();
}

bool DdsScene::layerVisible(const std::string &name) const
{
    const auto it = layers.find(name);
    return it != layers.end() && it->second.visible;
}

std::vector<std::string> DdsScene::layerNames() const
{
    std::vector<std::string> names;
    names.reserve(layers.size());
    for (const auto &[name, layer] : layers)
    {
        (void)layer;
        names.push_back(name);
    }
    std::sort(names.begin(), names.end());
    return names;
}

bool DdsScene::setObjectLayer(const std::string &id, const std::string &layerName)
{
    auto obj = findById(id);
    if (!obj || !layerExists(layerName))
        return false;
    const auto type = obj->type;
    if (type == DdsObjectType::Camera ||
        type == DdsObjectType::Ucs ||
        type == DdsObjectType::WorldExtentsObj)
        return false;
    if (obj->layerName != layerName)
    {
        obj->layerName = layerName;
        ++revision;
    }
    return true;
}

// ---------------------------------------------------------------------------
// DdsScene::findById
// ---------------------------------------------------------------------------

std::shared_ptr<DdsObject> DdsScene::findById(const std::string &id) const
{
    auto it = index.find(id);
    if (it == index.end() || it->second->deleted)
        return nullptr;
    return it->second;
}

// ---------------------------------------------------------------------------
// DdsScene::findByLabel
// ---------------------------------------------------------------------------

std::shared_ptr<DdsObject> DdsScene::findByLabel(const std::string &label) const
{
    if (label.empty())
        return nullptr;
    for (const auto &id : order)
    {
        auto it = index.find(id);
        if (it != index.end() && !it->second->deleted && it->second->label == label)
            return it->second;
    }
    return nullptr;
}

// ---------------------------------------------------------------------------
// DdsScene::findIdByLabel
// ---------------------------------------------------------------------------

std::string DdsScene::findIdByLabel(const std::string &label) const
{
    auto obj = findByLabel(label);
    return obj ? obj->id : std::string{};
}

// ---------------------------------------------------------------------------
// DdsScene::remove (soft-delete)
// ---------------------------------------------------------------------------

bool DdsScene::remove(const std::string &id)
{
    auto it = index.find(id);
    if (it == index.end())
        return false;
    it->second->deleted = true;
    ++revision;
    return true;
}

// ---------------------------------------------------------------------------
// DdsScene::clearDrawingObjects
// ---------------------------------------------------------------------------

void DdsScene::clearDrawingObjects()
{
    // Soft-delete all non-config objects.
    for (const auto &id : order)
    {
        auto it = index.find(id);
        if (it == index.end())
            continue;
        const auto t = it->second->type;
        if (t != DdsObjectType::Camera &&
            t != DdsObjectType::Ucs &&
            t != DdsObjectType::WorldExtentsObj)
        {
            it->second->deleted = true;
        }
    }
    compact();
    ++revision;
}

// ---------------------------------------------------------------------------
// DdsScene::clearAll
// ---------------------------------------------------------------------------

void DdsScene::clearAll()
{
    order.clear();
    index.clear();
    nextId = 1;
    ++revision;
}

// ---------------------------------------------------------------------------
// DdsScene::compact
// ---------------------------------------------------------------------------

void DdsScene::compact()
{
    // Remove soft-deleted entries from both collections.
    order.erase(
        std::remove_if(order.begin(), order.end(),
                       [this](const std::string &id) {
                           auto it = index.find(id);
                           return it == index.end() || it->second->deleted;
                       }),
        order.end());

    for (auto it = index.begin(); it != index.end();)
    {
        if (it->second->deleted)
            it = index.erase(it);
        else
            ++it;
    }
}

// ---------------------------------------------------------------------------
// DdsScene::count
// ---------------------------------------------------------------------------

int DdsScene::count() const
{
    int n = 0;
    for (const auto &id : order)
    {
        auto it = index.find(id);
        if (it != index.end() && !it->second->deleted)
            ++n;
    }
    return n;
}

// ---------------------------------------------------------------------------
// DdsScene::countDrawing
// ---------------------------------------------------------------------------

int DdsScene::countDrawing() const
{
    int n = 0;
    for (const auto &id : order)
    {
        auto it = index.find(id);
        if (it == index.end() || it->second->deleted)
            continue;
        const auto t = it->second->type;
        if (t != DdsObjectType::Camera &&
            t != DdsObjectType::Ucs &&
            t != DdsObjectType::WorldExtentsObj)
        {
            ++n;
        }
    }
    return n;
}

// ---------------------------------------------------------------------------
// DdsScene::idAt
// ---------------------------------------------------------------------------

std::string DdsScene::idAt(int idx) const
{
    int n = 0;
    for (const auto &id : order)
    {
        auto it = index.find(id);
        if (it != index.end() && !it->second->deleted)
        {
            if (n == idx)
                return id;
            ++n;
        }
    }
    return {};
}

bool appendDdsObject(std::shared_ptr<DdsObject> obj, int rb, const char *layerName)
{
    if (rb != 0)
    {
        gState.lastResult = grOk;
        return false;
    }

    if (gState.dds == nullptr)
    {
        gState.lastResult = grError;
        return false;
    }

    const std::string selectedLayer =
        (layerName != nullptr && layerName[0] != '\0') ? layerName : "DefaultLayer";
    if (!gState.dds->layerExists(selectedLayer))
    {
        gState.lastResult = grInvalidInput;
        return false;
    }

    obj->layerName = selectedLayer;
    gState.dds->append(std::move(obj));
    gState.lastResult = grOk;
    return true;
}

} // namespace bgi
