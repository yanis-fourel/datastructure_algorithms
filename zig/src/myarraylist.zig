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

        pub fn deinit(self: Self) void {
            self.alloc.free(self.items.ptr[0..self.capacity]);
        }

        pub fn append(self: *Self, val: T) !void {
            try self.ensureCapacity(self.items.len + 1);
            self.items.len += 1;
            self.items[self.items.len - 1] = val;
        }

        pub fn appendSlice(self: *Self, slice: []const T) !void {
            const newlen = self.items.len + slice.len;
            try self.ensureCapacity(newlen);
            @memcpy(self.items.ptr[self.items.len..newlen], slice);
            self.items.len += slice.len;
        }

        pub fn clear(self: *Self) void {
            self.items.len = 0;
        }

        pub fn pop(self: *Self) ?T {
            if (self.items.len == 0) {
                return null;
            }
            const res = self.items[self.items.len - 1];
            self.items.len -= 1;
            return res;
        }

        pub fn grow(self: *Self, new_capacity: usize) !void {
            const newslice = try self.alloc.alloc(T, new_capacity);

            @memcpy(newslice[0..self.items.len], self.items);
            self.alloc.free(self.items.ptr[0..self.capacity]);
            self.items.ptr = newslice.ptr;
            self.capacity = newslice.len;
        }

        pub fn ensureCapacity(self: *Self, capacity: usize) !void {
            if (capacity <= self.capacity) {
                return;
            }
            var newcapacity = self.capacity;
            while (newcapacity < capacity) {
                newcapacity = (newcapacity + 1) * 2;
            }
            try self.grow(newcapacity);
        }

        pub fn empty(self: Self) bool {
            return self.items.len == 0;
        }

        /// Returns the index of the first occurence of a value
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

        pub fn isSorted(self: Self) bool {
            if (self.items.len <= 1) {
                return true;
            }
            for (self.items[0 .. self.items.len - 1], self.items[1..]) |a, b| {
                if (a > b) {
                    return false;
                }
            }
            return true;
        }

        pub fn revert(self: *Self) void {
            var tmp: T = undefined;
            for (0..self.items.len / 2) |idx| {
                const mirroridx = self.items.len - 1 - idx;
                tmp = self.items[idx];
                self.items[idx] = self.items[mirroridx];
                self.items[mirroridx] = tmp;
            }
        }

        pub fn clone(self: Self) !Self {
            const mem = try self.alloc.dupe(T, self.items.ptr[0..self.capacity]);
            return Self{
                .items = mem.ptr[0..self.items.len],
                .capacity = mem.len,
                .alloc = self.alloc,
            };
        }

        /// Returns the index of the first occurence of a value
        /// Asserts arraylist is sorted
        pub fn binarySearch(self: Self, val: T) ?usize {
            assert(self.isSorted());

            if (self.empty()) return null;

            var lo: usize = 0;
            var hi: usize = self.items.len;
            var mid: usize = undefined;
            while (lo < hi) {
                mid = (lo + hi) / 2;
                if (self.items[mid] >= val) {
                    hi = mid;
                } else {
                    lo = mid + 1;
                }
            }
            if (lo < self.items.len and self.items[lo] == val) return lo;
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

test "issorted" {
    const alloc = std.testing.allocator;
    var al = try MyArrayList(u8).init(alloc);
    defer al.deinit();

    try expect(al.isSorted());
    try al.append(50);
    try expect(al.isSorted());
    try al.append(0);
    try expect(!al.isSorted());
    _ = al.pop();
    try al.append(51);
    try expect(al.isSorted());
    try al.append(49);
    try expect(!al.isSorted());
    _ = al.pop();
    try al.append(100);
    try expect(al.isSorted());
}

test "revert" {
    const alloc = std.testing.allocator;
    var al = try MyArrayList(u8).init(alloc);
    defer al.deinit();

    al.revert();
    try al.append(0);
    al.revert();
    try std.testing.expectEqualSlices(u8, &[_]u8{0}, al.items);
    try al.append(1);
    al.revert();
    try std.testing.expectEqualSlices(u8, &[_]u8{ 1, 0 }, al.items);
    try al.append(2);
    al.revert();
    try std.testing.expectEqualSlices(u8, &[_]u8{ 2, 0, 1 }, al.items);
    try al.append(3);
    al.revert();
    try std.testing.expectEqualSlices(u8, &[_]u8{ 3, 1, 0, 2 }, al.items);
    try al.append(4);
    al.revert();
    try std.testing.expectEqualSlices(u8, &[_]u8{ 4, 2, 0, 1, 3 }, al.items);
}

test "clone" {
    const alloc = std.testing.allocator;
    var al = try MyArrayList(u8).init(alloc);
    defer al.deinit();

    try al.append(0);
    try al.append(1);
    try al.append(2);
    try al.append(3);
    try al.append(4);

    const al2 = try al.clone();
    defer al2.deinit();
    try std.testing.expectEqualSlices(u8, al2.items, al.items);
    try expect(al2.items.ptr != al.items.ptr);
    try expectEqual(al2.capacity, al.capacity);
}

test "binarySearch" {
    const alloc = std.testing.allocator;
    var al = try MyArrayList(u8).init(alloc);
    defer al.deinit();

    try expectEqual(null, al.binarySearch(0));

    try al.append(0);
    try expectEqual(0, al.binarySearch(0));
    try expectEqual(null, al.binarySearch(1));
    try al.append(1);
    try expectEqual(0, al.binarySearch(0));
    try expectEqual(1, al.binarySearch(1));
    try expectEqual(null, al.binarySearch(2));
    try al.append(2);
    try expectEqual(0, al.binarySearch(0));
    try expectEqual(1, al.binarySearch(1));
    try expectEqual(2, al.binarySearch(2));
    try expectEqual(null, al.binarySearch(3));
    try al.append(3);
    try expectEqual(0, al.binarySearch(0));
    try expectEqual(1, al.binarySearch(1));
    try expectEqual(2, al.binarySearch(2));
    try expectEqual(3, al.binarySearch(3));
    try expectEqual(null, al.binarySearch(4));
    try al.append(4);
    try expectEqual(0, al.binarySearch(0));
    try expectEqual(1, al.binarySearch(1));
    try expectEqual(2, al.binarySearch(2));
    try expectEqual(3, al.binarySearch(3));
    try expectEqual(4, al.binarySearch(4));
    try expectEqual(null, al.binarySearch(5));
    try al.append(5);
    try expectEqual(0, al.binarySearch(0));
    try expectEqual(1, al.binarySearch(1));
    try expectEqual(2, al.binarySearch(2));
    try expectEqual(3, al.binarySearch(3));
    try expectEqual(4, al.binarySearch(4));
    try expectEqual(5, al.binarySearch(5));
    try expectEqual(null, al.binarySearch(6));

    try al.append(8);
    try al.append(8);
    try al.append(8);
    try al.append(8);
    try al.append(8);

    try expectEqual(0, al.binarySearch(0));
    try expectEqual(1, al.binarySearch(1));
    try expectEqual(2, al.binarySearch(2));
    try expectEqual(3, al.binarySearch(3));
    try expectEqual(4, al.binarySearch(4));
    try expectEqual(5, al.binarySearch(5));
    try expectEqual(6, al.binarySearch(8));
}

test "appendSlice" {
    const alloc = std.testing.allocator;
    var al = try MyArrayList(u8).init(alloc);
    defer al.deinit();

    try al.appendSlice(&[_]u8{});
    try std.testing.expectEqualSlices(u8, &[_]u8{}, al.items);
    try al.appendSlice(&[_]u8{ 1, 2, 3 });
    try std.testing.expectEqualSlices(u8, &[_]u8{ 1, 2, 3 }, al.items);
    try al.appendSlice(&[_]u8{ 4, 5, 6 });
    try std.testing.expectEqualSlices(u8, &[_]u8{ 1, 2, 3, 4, 5, 6 }, al.items);
    al.clear();
    try al.appendSlice(&[_]u8{10});
    try std.testing.expectEqualSlices(u8, &[_]u8{10}, al.items);
    try al.appendSlice(&[_]u8{0});
    try std.testing.expectEqualSlices(u8, &[_]u8{ 10, 0 }, al.items);
}
