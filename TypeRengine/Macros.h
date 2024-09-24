#ifndef MACROS_H
#define MACROS_H

#define DEFINE_GET_COMPONENT(CustomName, ComponentType) \
    __forceinline ComponentType* get##CustomName() { return Context.tryGetComponent<ComponentType>(); }

#define DEFINE_HAS_COMPONENT(CustomName, ComponentType) \
    __forceinline bool has##CustomName() { return Context.hasComponent<ComponentType>(); }

#define DEFINE_IS_COMPONENT(CustomName, ComponentType) \
    __forceinline bool is##CustomName() { return Context.hasComponent<FlagComponent<ComponentType>>(); }

// __forceinline bool isAutoResize() { return Context.hasComponent<FlagComponent<AutoResizeFlag>>(); }

#endif