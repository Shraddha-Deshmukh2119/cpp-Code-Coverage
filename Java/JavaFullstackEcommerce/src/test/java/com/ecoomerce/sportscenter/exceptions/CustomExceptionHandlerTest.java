package com.ecoomerce.sportscenter.exceptions;

import org.junit.jupiter.api.Test;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.mock.web.MockHttpServletRequest;
import org.springframework.web.context.request.ServletWebRequest;

import static org.assertj.core.api.Assertions.assertThat;

class CustomExceptionHandlerTest {

    private final CustomExceptionHandler handler = new CustomExceptionHandler();

    @Test
    void handleProductNotFoundException_returnsNotFound() {
        ProductNotFoundException ex = new ProductNotFoundException("Product missing");
        ServletWebRequest webRequest = new ServletWebRequest(new MockHttpServletRequest());

        ResponseEntity<Object> response = handler.handleProductNotFoundException(ex, webRequest);

        assertThat(response.getStatusCode()).isEqualTo(HttpStatus.NOT_FOUND);
        assertThat(response.getBody()).isNotNull();
    }
}
