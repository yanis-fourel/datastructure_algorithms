const std = @import("std");
const Allocator = std.mem.Allocator;
const assert = std.debug.assert;

pub fn MyRingBuffer(comptime T: type) type {
    return struct {
        const Self = @This();

        allocator: Allocator,
        data: []T,
        /// [0; len], index of the first element
        first: usize,
        /// [0; len], index after the last element
        last: usize,
        // if first == last, then either len == 0 or len == data.len
        len: usize,

        pub fn init(allocator: Allocator) Self {
            return Self{
                .allocator = allocator,
                .data = &[_]T{},
                .first = 0,
                .last = 0,
                .len = 0,
            };
        }

        pub fn deinit(self: Self) void {
            self.allocator.free(self.data);
        }

        pub fn empty(self: Self) bool {
            return self.len == 0;
        }

        pub fn prepend(self: *Self, item: T) !void {
            try self.ensureCapacity(self.len + 1);
            self.first = self.idxBefore(self.first);
            self.data[self.first] = item;
            self.len += 1;
        }

        pub fn append(self: *Self, item: T) !void {
            try self.ensureCapacity(self.len + 1);
            self.data[self.last % self.data.len] = item;
            self.last = self.idxAfter(self.last);
            self.len += 1;
        }

        pub fn popFirst(self: *Self) ?T {
            if (self.empty()) {
                return null;
            }
            const oldfirst = self.first;
            self.first = self.idxAfter(self.first);
            self.len -= 1;
            return self.data[oldfirst];
        }

        pub fn popLast(self: *Self) ?T {
            if (self.empty()) {
                return null;
            }

            self.last = self.idxBefore(self.last);
            self.len -= 1;
            return self.data[self.last];
        }

        /// asserts idx < self.len
        pub fn at(self: Self, idx: usize) T {
            assert(idx < self.len);
            idx = (idx + self.first) % self.data.len;
            assert(idx < self.last);
            return self.items[idx];
        }
        pub fn idxBefore(self: Self, idx: usize) usize {
            const res = (self.data.len + idx - 1) % self.data.len;
            assert(res >= 0);
            assert(res < self.data.len);
            return res;
        }
        pub fn idxAfter(self: Self, idx: usize) usize {
            const res = (idx + 1) % self.data.len;
            assert(res >= 0);
            assert(res < self.data.len);
            return res;
        }

        pub fn ensureCapacity(self: *Self, capacity: usize) !void {
            if (capacity <= self.data.len) return;

            var newcapacity = self.data.len;
            while (newcapacity < capacity) : (newcapacity |= (newcapacity + 1) * 2) {}

            const newdata = try self.allocator.alloc(T, newcapacity);
            const newfirst = (newdata.len - self.len) / 2;
            const newlast = newfirst + self.len;
            if (self.first < self.last) {
                @memcpy(newdata[newfirst..newlast], self.data[self.first..self.last]);
            } else {
                const mid = newfirst + (self.data.len - self.first);
                @memcpy(newdata[newfirst..mid], self.data[self.first..]);
                @memcpy(newdata[mid..newlast], self.data[0..self.last]);
            }
            self.first = newfirst;
            self.last = newlast;
            self.allocator.free(self.data);
            self.data = newdata;
        }
    };
}

test "MyRingBuffer" {
    const allocator = std.testing.allocator;
    var rb = MyRingBuffer(u32).init(allocator);
    defer rb.deinit();

    try std.testing.expect(rb.empty());
    try std.testing.expectEqual(null, rb.popLast());
    try std.testing.expectEqual(null, rb.popFirst());
    try rb.append(10);
    try std.testing.expect(!rb.empty());
    try std.testing.expectEqual(10, rb.popLast());
    try std.testing.expect(rb.empty());
    try rb.append(10);
    try std.testing.expectEqual(10, rb.popFirst());
    try rb.prepend(10);
    try std.testing.expect(!rb.empty());
    try std.testing.expectEqual(10, rb.popLast());
    try std.testing.expect(rb.empty());
    try rb.prepend(10);
    try std.testing.expectEqual(10, rb.popFirst());

    try rb.append(10);
    try rb.append(11);
    try rb.append(12);
    try rb.append(13);
    try rb.append(14);
    try rb.append(15);
    try std.testing.expectEqual(10, rb.popFirst());
    try std.testing.expectEqual(15, rb.popLast());
    try std.testing.expectEqual(11, rb.popFirst());
    try std.testing.expectEqual(14, rb.popLast());
    try std.testing.expectEqual(12, rb.popFirst());
    try std.testing.expectEqual(13, rb.popLast());
    try std.testing.expectEqual(null, rb.popLast());
    try std.testing.expectEqual(null, rb.popFirst());
    try std.testing.expect(rb.empty());

    try rb.prepend(15);
    try rb.prepend(14);
    try rb.prepend(13);
    try rb.prepend(12);
    try rb.prepend(11);
    try rb.prepend(10);
    try std.testing.expectEqual(10, rb.popFirst());
    try std.testing.expectEqual(15, rb.popLast());
    try std.testing.expectEqual(11, rb.popFirst());
    try std.testing.expectEqual(14, rb.popLast());
    try std.testing.expectEqual(12, rb.popFirst());
    try std.testing.expectEqual(13, rb.popLast());
    try std.testing.expectEqual(null, rb.popLast());
    try std.testing.expectEqual(null, rb.popFirst());
    try std.testing.expect(rb.empty());

    try rb.append(13);
    try rb.append(14);
    try rb.prepend(12);
    try rb.prepend(11);
    try rb.append(15);
    try rb.prepend(10);
    try std.testing.expectEqual(10, rb.popFirst());
    try std.testing.expectEqual(15, rb.popLast());
    try std.testing.expectEqual(11, rb.popFirst());
    try std.testing.expectEqual(14, rb.popLast());
    try std.testing.expectEqual(12, rb.popFirst());
    try std.testing.expectEqual(13, rb.popLast());
    try std.testing.expectEqual(null, rb.popLast());
    try std.testing.expectEqual(null, rb.popFirst());
    try std.testing.expect(rb.empty());

    var i: u32 = 0;
    while (i < 50) : (i += 1) {
        try rb.append(i);
    }
    while (i < 1024) : (i += 1) {
        try rb.append(i);
        try std.testing.expectEqual(i - 50, rb.popFirst());
    }
    while (i < 1024 + 50) : (i += 1) {
        try std.testing.expectEqual(i - 50, rb.popFirst());
    }
    try std.testing.expect(rb.empty());

    i -= 1;
    while (i >= 1024) : (i -= 1) {
        try rb.prepend(i);
    }
    while (i >= 50) : (i -= 1) {
        try rb.prepend(i);
        try std.testing.expectEqual(i + 50, rb.popLast());
    }
    while (i > 0) : (i -= 1) {
        try std.testing.expectEqual(i + 50, rb.popLast());
    }
    try std.testing.expectEqual(50, rb.popLast());
    try std.testing.expectEqual(null, rb.popLast());
    try std.testing.expect(rb.empty());
}
