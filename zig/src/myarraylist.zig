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

        /// Returns the index of the first occurence of a value, optionnaly
        pub fn find(self: Self, val: T) ?usize {
            return self.findFrom(val, 0);
        }

        /// Returns the index of the first occurence of a value starting from index
        pub fn findFrom(self: Self, val: T, startidx: usize) ?usize {
            if (startidx < self.items.len) {
                for (self.items[startidx..], startidx..) |v, i| {
                    if (v == val) {
                        return i;
                    }
                }
            }
            return null;
        }
    };
}

const expect = std.testing.expect;
const expectEqual = std.testing.expectEqual;

test "MyArrayList" {
    const alloc = std.testing.allocator;
    var al = try MyArrayList(u8).init(alloc);
    defer al.deinit();

    try expectEqual(null, al.pop());
    try al.append(42);
    try expectEqual(42, al.pop());
    try expectEqual(null, al.pop());

    {
        var i: u8 = 0;
        while (i < 100) : (i += 1) {
            try al.append(i);
        }
    }

    for (al.items, 0..) |val, idx| {
        try expectEqual(idx, val);
    }
}

test "find" {
    const alloc = std.testing.allocator;
    var al = try MyArrayList(u8).init(alloc);
    defer al.deinit();

    try expectEqual(null, al.find(0));
    try expectEqual(null, al.find(42));
    try al.append(42);
    try expectEqual(null, al.find(0));
    try expectEqual(0, al.find(42));
    try al.append(42);
    try expectEqual(null, al.find(0));
    try expectEqual(0, al.find(42));
    try al.append(1);
    try al.append(3);
    try al.append(11);
    try expectEqual(null, al.find(0));
    try expectEqual(0, al.find(42));
    try expectEqual(2, al.find(1));
    try expectEqual(3, al.find(3));
    try expectEqual(4, al.find(11));
}

test "findFrom" {
    const alloc = std.testing.allocator;
    var al = try MyArrayList(u8).init(alloc);
    defer al.deinit();

    try expectEqual(null, al.findFrom(0, 0));
    try expectEqual(null, al.findFrom(0, 100));
    try expectEqual(null, al.findFrom(42, 0));
    try al.append(42);
    try expectEqual(null, al.findFrom(0, 100));
    try expectEqual(0, al.findFrom(42, 0));
    try expectEqual(null, al.findFrom(42, 1));
    try al.append(42);
    try expectEqual(null, al.findFrom(0, 0));
    try expectEqual(null, al.findFrom(0, 100));
    try expectEqual(0, al.findFrom(42, 0));
    try expectEqual(1, al.findFrom(42, 1));
    try expectEqual(null, al.findFrom(42, 2));
    try expectEqual(null, al.findFrom(42, 100));
    try al.append(1);
    try al.append(3);
    try al.append(11);
    try expectEqual(null, al.findFrom(0, 0));
    try expectEqual(null, al.findFrom(0, 3));
    try expectEqual(0, al.findFrom(42, 0));
    try expectEqual(1, al.findFrom(42, 1));
    try expectEqual(null, al.findFrom(42, 2));
    try expectEqual(2, al.findFrom(1, 0));
    try expectEqual(2, al.findFrom(1, 1));
    try expectEqual(2, al.findFrom(1, 2));
    try expectEqual(null, al.findFrom(1, 3));
    try expectEqual(3, al.findFrom(3, 0));
    try expectEqual(3, al.findFrom(3, 1));
    try expectEqual(3, al.findFrom(3, 2));
    try expectEqual(3, al.findFrom(3, 3));
    try expectEqual(null, al.findFrom(3, 4));
    try expectEqual(4, al.findFrom(11, 0));
    try expectEqual(4, al.findFrom(11, 1));
    try expectEqual(4, al.findFrom(11, 2));
    try expectEqual(4, al.findFrom(11, 3));
    try expectEqual(4, al.findFrom(11, 4));
    try expectEqual(null, al.findFrom(11, 5));
}
