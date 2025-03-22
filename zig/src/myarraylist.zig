const std = @import("std");
const Allocator = std.mem.Allocator;

/// Contiguous growable memory buffer
pub fn MyArrayList(comptime t: type) type {
    return struct {
        const Self = @This();

        items: []t,
        size: u32,
        alloc: Allocator,

        pub fn init(alloc: Allocator) !Self {
            return Self{
                .items = try alloc.alloc(t, 8),
                .size = 0,
                .alloc = alloc,
            };
        }

        pub fn deinit(self: *Self) void {
            self.alloc.free(self.items);
        }

        pub fn push(self: *Self, val: t) !void {
            if (self.items.len == self.size) {
                try self.grow();
            }
            self.items[self.size] = val;
            self.size += 1;
        }

        pub fn pop(self: *Self) ?t {
            if (self.size == 0) {
                return null;
            }
            self.size -= 1;
            return self.items[self.size];
        }

        pub fn grow(self: *Self) !void {
            const newlen = self.size * 2;
            self.items = try self.alloc.realloc(self.items, newlen);
            self.size = newlen;
        }
    };
}

const expect = std.testing.expect;

test "MyArrayList" {
    const alloc = std.testing.allocator;
    var al = try MyArrayList(u8).init(alloc);
    defer al.deinit();

    try expect(al.pop() == null);
    try al.push(42);
    try expect(al.pop() == 42);
    try expect(al.pop() == null);
}
