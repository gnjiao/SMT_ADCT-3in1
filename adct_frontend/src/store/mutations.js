export const setToken = (state, token) => {
    state.token = token;
};

export const setPage = (state, page) => {
    state.page.reviewById = page.reviewById;
    state.page.continueById = page.continueById;
};