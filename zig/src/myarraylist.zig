const std = @import("std");
const Allocator = std.mem.Allocator;
const print = std.debug.print;
const assert = std.debug.assert;

/// Contiguous growable memory buffer
pub fn MyArrayList(comptime T: type) type {
    return struct {
        const Self = @This();

        comptime {
            assert(@sizeOf(T) != 0);
        }

        items: []T,
        capacity: usize,
        alloc: Allocator,

        pub fn init(alloc: Allocator) !Self {
            return Self{
                .capacity = 0,
                .items = &[_]T{},
                .alloc = alloc,
            };
        }

        pub fn deinit(self: *Self) void {
            self.items.len = self.capacity;
            self.alloc.free(self.items);
        }

        pub fn append(self: *Self, val: T) !void {
            if (self.items.len + 1 >= self.capacity) {
                try self.grow();
            }
            self.items.len += 1;
            self.items[self.items.len - 1] = val;
        }

        pub fn pop(self: *Self) ?T {
            if (self.items.len == 0) {
                return null;
            }
            const res = self.items[self.items.len - 1];
            self.items.len -= 1;
            return res;
        }

        pub fn grow(self: *Self) !void {
            const newlen = (self.capacity + 1) * 2;
            const newslice = try self.alloc.alloc(T, newlen);

            @memcpy(newslice[0..self.items.len], self.items);
            self.alloc.free(self.items.ptr[0..self.capacity]);
            self.items.ptr = newslice.ptr;
            self.capacity = newslice.len;
        }

        pub fn empty(self: Self) bool {
            return self.items.len == 0;
        }
    };
}

const expect = std.testing.expect;

test "MyArrayList" {
    const alloc = std.testing.allocator;
    var al = try MyArrayList(u8).init(alloc);
    defer al.deinit();

    try expect(al.pop() == null);
    try al.append(42);
    try expect(al.pop() == 42);
    try expect(al.pop() == null);

    var i: u8 = 0;
    while (i < 100) : (i += 1) {
        try al.append(i);
    }
}
