#include <kitsune/crashtrace.h>
#include <kitsune/hashes.h>
#include <kitsune/iterator.h>
#include <kitsune/dynamic_iterator.h>
#include <kitsune/alloc/traced.h>
#include <kitsune/map.h>
#include <kitsune/allocator.h>
#include <kitsune/numbers.h>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define STACK_SIZE 16

struct allocator_ctx {
        struct kitsune_allocator *base;
        struct kitsune_map pointers;
};

struct pointer_trace {
        void *stack[STACK_SIZE];
        usize size;
};

static void *
allocate(struct kitsune_allocator *a, usize size)
{
        struct allocator_ctx *ctx = a->context;

        void *allocated = ctx->base->alloc(ctx->base, size);
        struct pointer_trace trace = {0};
        trace.size = backtrace(trace.stack, STACK_SIZE);

        kitsune_map_insert(&ctx->pointers, &allocated, sizeof(void**),
            &trace);

        return allocated;
}

static void *
reallocate(struct kitsune_allocator *a, void *ptr, usize size)
{
        struct allocator_ctx *ctx = a->context;
        void *reallocated = ctx->base->resize(ctx->base, ptr, size);
        if (ptr != reallocated) {
                struct pointer_trace* trace = kitsune_map_remove(&ctx->pointers,
                    &ptr, sizeof(void**));
                kitsune_map_insert(&ctx->pointers, &reallocated, sizeof(void**)
                    , trace);
                ctx->base->free(ctx->base, trace);
        }
        
        return reallocated;
}

static void
destroy(struct kitsune_allocator *a, void *ptr)
{
        if (ptr == NULL)
                return;

        struct allocator_ctx *ctx = a->context;
        struct pointer_trace *trace = kitsune_map_remove(&ctx->pointers,
            &ptr, sizeof(void**));

        ctx->base->free(ctx->base, ptr);
        ctx->base->free(ctx->base, trace);
}

#undef STACK_SIZE

struct kitsune_traced_allocator
kitsune_traced_allocator_init(struct kitsune_allocator *allocator)
{
        struct kitsune_traced_allocator traced = {0};
        traced.base = allocator;

        return traced;
}

void
kitsune_traced_allocator_deinit(struct kitsune_traced_allocator *traced)
{
        bool leaked = false;

        if (traced->allocator.context == NULL)
            return;

        struct allocator_ctx *ctx = traced->allocator.context;

        struct kitsune_dynamic_iterator iter = kitsune_map_iterator(
            &ctx->pointers);

        struct kitsune_map_entry *entry = kitsune_iterator_next(&iter.base);

        if (entry != NULL)
                leaked = true;

        for (; entry != NULL; entry = kitsune_iterator_next(&iter.base)) {
                struct pointer_trace *trace = entry->value;
                if (trace->size == 0) continue;
                fprintf(stderr, 
                    "Memory leaked at address %p!\n━━━━━━━━━━━━━━━\n"
                    , *(void**) entry->key);
                kitsune_print_backtrace(trace->size, trace->stack, true);
                fprintf(stderr, "━━━━━━━━━━━━━━━\n");
        }

        kitsune_dynamic_iterator_deinit(&iter);
        kitsune_map_deinit(&ctx->pointers, NULL);
        ctx->base->free(ctx->base, traced->allocator.context);

        if (leaked) {
                fprintf(stderr, "*** memory leak detected ***: terminated\n");
                abort();
        }
}

struct kitsune_allocator*
kitsune_traced_allocator_allocator(struct kitsune_traced_allocator *traced)
{
        struct allocator_ctx *ctx = traced->base->alloc(traced->base,
            sizeof(struct allocator_ctx));
        ctx->base = traced->base;
        ctx->pointers = kitsune_map_init(sizeof(struct pointer_trace),
            ctx->base, kitsune_fnv1a_64);

        traced->allocator = (struct kitsune_allocator) {
            .alloc = allocate, .resize = reallocate, .free = destroy,
            .context = ctx };

        return &traced->allocator;
}
