package com.ecoomerce.sportscenter.exceptions;

import org.junit.jupiter.api.Test;

import static org.assertj.core.api.Assertions.assertThat;

class ProductNotFoundExceptionTest {

    @Test
    void constructor_setsMessage() {
        ProductNotFoundException ex = new ProductNotFoundException("not found");

        assertThat(ex.getMessage()).isEqualTo("not found");
    }
}
